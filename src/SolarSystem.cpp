//
// Created by lucas on 7/26/25.
//

#include "SolarSystem.h"

#include <spdlog/spdlog.h>
#include <raymath.h>

#include "EntityComponentSystem/Components/Components.h"
#include "EntityComponentSystem/Systems/RenderingSystem.h"
#include "Renderer/Renderer.h"
#include "Config.h"
#include "Renderer/Bloom.h"

namespace SS3D
{
    SolarSystem::SolarSystem(EntityComponentSystem& ecs, const std::filesystem::path& configurationFile) :
        ecs(ecs),
        componentsRegister(ecs.componentsRegister),
        entityManager(ecs.entityManager)
    {
        const Config config{configurationFile};

        modelScale = static_cast<float>(config.modelScale);
        resourcePath = config.resourcePath;
        ecs.componentsRegister->registerComponentType<SS3D::Transform>();
        ecs.componentsRegister->registerComponentType<Motion>();
        ecs.componentsRegister->registerComponentType<Graphics>();
        ecs.componentsRegister->registerComponentType<Light>();
        ecs.componentsRegister->registerComponentType<Orbiting>();
        ecs.componentsRegister->registerComponentType<Camera>();

        const auto renderer = std::make_shared<Renderer::Renderer>(config.width, config.height);
        renderer->initialize(config.shaderPath);
        renderer->setupSkybox(resourcePath / config.skyboxTexturePath);
        auto renderingSystem = ecs.systemRegister->registerSystem<RenderingSystem>(Signature("00000101"), renderer);
        auto lightingSystem = ecs.systemRegister->registerSystem<LightingSystem>(Signature("00001001"), renderer);
        auto motionSystem = ecs.systemRegister->registerSystem<MovementSystem>(Signature("00000011"));
        auto controlsSystem = ecs.systemRegister->registerSystem<ControlsSystem>(Signature("00100001"), renderer);
        auto physicsSystem = ecs.systemRegister->registerSystem<PhysicsSystem>(
            Signature("00010011"), 0.1f, 6.67259e-11, modelScale);

        renderingSystem->initialize();
        lightingSystem->initialize();
        physicsSystem->initialize();
        motionSystem->initialize();
        controlsSystem->initialize();

        const auto cameraEntity = entityManager->createEntity("camera", "camera");
        componentsRegister->addComponent<Transform>(cameraEntity, SS3D::Transform{
                                                        .position = Vector3(0.0f, 0.0f, 0.0f),
                                                    });
        componentsRegister->addComponent<Camera>(cameraEntity, SS3D::Camera{});

        setSystem(config.bodySpawnConfigs);
        setModels(config.modelSpawnConfigs);

        renderer->addPostProcessing<Bloom>("bloom");
        spdlog::info("Solar System Initialized");
    }


    void SolarSystem::setSystem(const std::vector<BodySpawnConfig>& config)
    {
        for (const auto& bodySpawnConfig : config)
        {
            createBody(bodySpawnConfig);
        }
    }

    void SolarSystem::setModels(const std::vector<ModelSpawnConfig>& config)
    {
        for (const auto& model_spawn_config : config)
        {
            createModel(model_spawn_config);
        }
    }

    void SolarSystem::getGlobalTransformMotion(const SpawnConfig& config, std::optional<ComponentInstance>& refBody,
                                               Vector3& currentPosition, Quaternion& currentAttitude,
                                               Vector3& currentVelocity,
                                               Quaternion& currentRotationSpeed) const
    {
        const auto refBodyEntity = *entityManager->getEntityByName(config.refBodyName);
        refBody = componentsRegister->getComponentInstance<Orbiting>(refBodyEntity);
        currentPosition = config.position * modelScale;
        currentAttitude = config.attitude;
        currentRotationSpeed = QuaternionFromAxisAngle(Vector3Normalize(config.rotationSpeed),
                                                            Vector3Length(config.rotationSpeed));
        currentVelocity = config.speed * modelScale;
        auto currentRefBody = refBody;

        while (currentRefBody.has_value())
        {
            const auto orbitingInstance = currentRefBody;
            const auto refEntity = componentsRegister->getComponentCollection<Orbiting>()->getEntity(
                *orbitingInstance);
            const auto& refTransform = componentsRegister->getComponent<Transform>(refEntity);
            const auto& refVelocity = componentsRegister->getComponent<Motion>(refEntity);

            const auto angle = Vector3Length(refVelocity.rotationSpeed);
            const auto axis = Vector3Normalize(refVelocity.rotationSpeed);
            const auto addedRotation = QuaternionFromAxisAngle(axis, angle);

            currentPosition = refTransform.position + Vector3RotateByQuaternion(currentPosition, addedRotation);
            currentAttitude = QuaternionMultiply(refTransform.rotation, currentAttitude);

            currentRotationSpeed = QuaternionMultiply(addedRotation, currentRotationSpeed);
            currentVelocity = refVelocity.velocity + Vector3RotateByQuaternion(currentVelocity, addedRotation) +
                Vector3CrossProduct(refVelocity.rotationSpeed, refTransform.position);

            currentRefBody = componentsRegister->getComponent<Orbiting>(refEntity).refBody;
        }
    }

    Entity SolarSystem::createBody(const BodySpawnConfig& config)
    {
        const auto bodyEntity = entityManager->createEntity(config.name, "body");
        bodies[config.name] = bodyEntity;

        std::optional<ComponentInstance> refBody{std::nullopt};
        if (config.refBodyName.empty())
        {
            componentsRegister->addComponent<Transform>(bodyEntity, SS3D::Transform{
                                                            .position = Vector3Scale(config.position, modelScale),
                                                            .rotation = config.attitude,
                                                            .scale = config.radius * config.scale * modelScale,
                                                        });

            componentsRegister->addComponent<Motion>(bodyEntity, SS3D::Motion{
                                                         .velocity = config.speed * modelScale,
                                                         .rotationSpeed = config.rotationSpeed,
                                                     });
        }
        else
        {
            Vector3 currentPosition;
            Quaternion currentAttitude;
            Vector3 currentVelocity;
            Quaternion currentRotationSpeed;
            getGlobalTransformMotion(config, refBody, currentPosition, currentAttitude, currentVelocity , currentRotationSpeed);
            componentsRegister->addComponent<Transform>(bodyEntity, SS3D::Transform{
                                                            .position = currentPosition,
                                                            .rotation = currentAttitude,
                                                            .scale = config.radius * config.scale * modelScale,
                                                        });

            Vector3 finalAxis{};
            float finalAngle{};
            QuaternionToAxisAngle(currentRotationSpeed, &finalAxis, &finalAngle);
            const Vector3 finalRotationSpeed = finalAxis * finalAngle;
            componentsRegister->addComponent<Motion>(bodyEntity, SS3D::Motion{
                                                         .velocity = currentVelocity,
                                                         .rotationSpeed = finalRotationSpeed,
                                                     });
        }


        Material material;
        makeMaterial(config.name, material, config.shaderName);
        const auto model = LoadModel((resourcePath / "Planet.glb").c_str());
        GenMeshTangents(&model.meshes[0]);


        componentsRegister->addComponent<Graphics>(bodyEntity, SS3D::Graphics{
                                                       .type = GraphicsType::SPHERE,
                                                       .material = material,
                                                       .model = model,
                                                       .renderParameters = config.renderParameters,
                                                   });

        componentsRegister->addComponent<Orbiting>(bodyEntity, SS3D::Orbiting{
                                                       .mass = config.mass,
                                                       .refBody = refBody,
                                                   });

        if (config.isLight)
        {
            componentsRegister->addComponent<Light>(
                bodyEntity, SS3D::Light{.handle = 1, .power = static_cast<float>(config.lightIntensity)});
        }

        spdlog::info("Created {}", config.name);
        return bodyEntity;
    }

    Entity SolarSystem::createModel(const ModelSpawnConfig& config)
    {
        const auto bodyEntity = entityManager->createEntity(config.name, "model");
        bodies[config.name] = bodyEntity;

        std::optional<ComponentInstance> refBody{std::nullopt};
        if (config.refBodyName.empty())
        {
            componentsRegister->addComponent<Transform>(bodyEntity, SS3D::Transform{
                                                            .position = Vector3Scale(config.position, modelScale),
                                                            .rotation = config.attitude,
                                                            .scale = config.scale * modelScale,
                                                        });

            componentsRegister->addComponent<Motion>(bodyEntity, SS3D::Motion{
                                                         .velocity = config.speed * modelScale,
                                                         .rotationSpeed = config.rotationSpeed,
                                                     });
        }
        else
        {
            Vector3 currentPosition;
            Quaternion currentAttitude;
            Vector3 currentVelocity;
            Quaternion currentRotationSpeed;
            getGlobalTransformMotion(config, refBody, currentPosition, currentAttitude, currentVelocity , currentRotationSpeed);
            componentsRegister->addComponent<Transform>(bodyEntity, SS3D::Transform{
                                                            .position = currentPosition,
                                                            .rotation = currentAttitude,
                                                            .scale = config.scale * modelScale,
                                                        });

            Vector3 finalAxis{};
            float finalAngle{};
            QuaternionToAxisAngle(currentRotationSpeed, &finalAxis, &finalAngle);
            const Vector3 finalRotationSpeed = finalAxis * finalAngle;
            componentsRegister->addComponent<Motion>(bodyEntity, SS3D::Motion{
                                                         .velocity = currentVelocity,
                                                         .rotationSpeed = finalRotationSpeed,
                                                     });
        }


        Material material;
        const auto model = LoadModel((resourcePath / config.modelPath).c_str());
        GenMeshTangents(&model.meshes[0]);


        componentsRegister->addComponent<Graphics>(bodyEntity, SS3D::Graphics{
                                                       .type = GraphicsType::MODEL,
                                                       .model = model,
                                                       .renderParameters = config.renderParameters,
                                                   });

        componentsRegister->addComponent<Orbiting>(bodyEntity, SS3D::Orbiting{
                                                       .mass = config.mass,
                                                       .refBody = refBody,
                                                   });

        if (config.isLight)
        {
            componentsRegister->addComponent<Light>(
                bodyEntity, SS3D::Light{.handle = 1, .power = static_cast<float>(config.lightIntensity)});
        }

        spdlog::info("Created {}", config.name);
        return bodyEntity;
    }


    void SolarSystem::update(const float deltaTime)
    {
        ecs.systemRegister->getSystem<ControlsSystem>()->update(deltaTime);
        ecs.systemRegister->getSystem<PhysicsSystem>()->update(deltaTime);
        ecs.systemRegister->getSystem<MovementSystem>()->update(deltaTime);
        ecs.systemRegister->getSystem<LightingSystem>()->update(deltaTime);
        ecs.systemRegister->getSystem<RenderingSystem>()->update(deltaTime);
    }

    void SolarSystem::render()
    {
        //TODO Clean that up in the systems
        ecs.systemRegister->getSystem<RenderingSystem>()->beforeRender();
        ecs.systemRegister->getSystem<PhysicsSystem>()->render();
        ecs.systemRegister->getSystem<RenderingSystem>()->render();
        ecs.systemRegister->getSystem<ControlsSystem>()->render();
        ecs.systemRegister->getSystem<LightingSystem>()->render();
        ecs.systemRegister->getSystem<MovementSystem>()->render();
        ecs.systemRegister->getSystem<RenderingSystem>()->afterRender();
    }

    void SolarSystem::run()
    {
        double lastTime = GetTime();
        float speedUp = 1.f;

        const auto controlSystem = ecs.systemRegister->getSystem<ControlsSystem>();
        controlSystem->parameterPointers["speedUp"] = static_cast<void*>(&speedUp);
        while (!WindowShouldClose())
        {
            const double newTime = GetTime();
            const auto deltaTime = 1.f / 60; //static_cast<float>(newTime - lastTime); //TODO uncomment
            update(static_cast<float>(deltaTime * std::pow(10, speedUp)));
            lastTime = newTime;
            render();
        }
    }


    //TODO should really be move to the renderer
    void SolarSystem::makeMaterial(const std::string& bodyName, Material& material, const std::string& shaderName) const
    {
        const auto texturesPath = resourcePath / "SolarTextures";
        const auto diffuseTexture = LoadTexture((texturesPath / (bodyName + "_diffuse.jpg")).c_str());
        const auto specularTexture = LoadTexture((texturesPath / (bodyName + "_specular.png")).c_str());
        const auto normalMap = LoadTexture((texturesPath / (bodyName + "_normal.png")).c_str());
        const auto nightTexture = LoadTexture((texturesPath / (bodyName + "_emit_night.jpg")).c_str());

        material = LoadMaterialDefault();
        material.shader = ecs.systemRegister->getSystem<RenderingSystem>()->renderer->getShader(shaderName);
        SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, diffuseTexture);
        SetMaterialTexture(&material, MATERIAL_MAP_SPECULAR, specularTexture);
        SetMaterialTexture(&material, MATERIAL_MAP_NORMAL, normalMap);
        SetMaterialTexture(&material, MATERIAL_MAP_EMISSION, nightTexture);
    }
}
