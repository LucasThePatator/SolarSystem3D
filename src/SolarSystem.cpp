//
// Created by lucas on 7/26/25.
//

#include "SolarSystem.h"

#include <spdlog/spdlog.h>
#include <raymath.h>

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

#include "EntityComponentSystem/Components/Components.h"
#include "EntityComponentSystem/Systems/RenderingSystem.h"
#include "Renderer/Renderer.h"


constexpr float modelScale = 1.f / 1000000;

namespace SS3D
{
    SolarSystem::SolarSystem(EntityComponentSystem& ecs, const std::filesystem::path& configurationFile) :
        ecs(ecs),
        componentsRegister(ecs.componentsRegister),
        entityManager(ecs.entityManager)
    {
        const toml::table configurationTbl = toml::parse_file(configurationFile.string());
        const auto resourcesTbl = *configurationTbl.at("resources").as_table();
        resourcePath = std::filesystem::path(*resourcesTbl.at("resourcePath").value<std::string>());

        ecs.componentsRegister->registerComponentType<SS3D::Transform>();
        ecs.componentsRegister->registerComponentType<Motion>();
        ecs.componentsRegister->registerComponentType<Graphics>();
        ecs.componentsRegister->registerComponentType<Light>();
        ecs.componentsRegister->registerComponentType<Orbiting>();
        ecs.componentsRegister->registerComponentType<Camera>();


        const auto renderer = std::make_shared<Renderer::Renderer>(1024, 768);
        renderer->initialize(*resourcesTbl.at("shaderPath").value<std::string>());
        auto renderingSystem = ecs.systemRegister->registerSystem<RenderingSystem>(Signature("00000101"), renderer);
        auto lightingSystem = ecs.systemRegister->registerSystem<LightingSystem>(Signature("00001001"), renderer);
        auto motionSystem = ecs.systemRegister->registerSystem<MovementSystem>(Signature("00000011"));
        auto controlsSystem = ecs.systemRegister->registerSystem<ControlsSystem>(Signature("00100001"), renderer);
        auto physicsSystem = ecs.systemRegister->registerSystem<PhysicsSystem>(
            Signature("00010011"), 0.1f, 6.67259e-29, modelScale);

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

        const auto sceneTable = *configurationTbl.at("scene").as_table();
        setSystem(sceneTable);
        spdlog::info("Solar System Initialized");
    }

    void SolarSystem::setSystem(const toml::table& tbl)
    {
        auto fillRaylibType = [](const toml::array* array, float* const raylibTypePtr)
        {
            for (size_t i = 0; i < array->size(); ++i)
            {
                auto& elem = (*array)[i];
                elem.visit([i, raylibTypePtr]<typename elType>(elType&& el)
                {
                    if constexpr (toml::is_integer<elType>)
                        raylibTypePtr[i] = static_cast<float>(**el.as_integer());
                    if constexpr (toml::is_floating_point<elType>)
                        raylibTypePtr[i] = static_cast<float>(**el.as_floating_point());
                });
            }
        };

        tbl.for_each([&]<typename EntryType>(EntryType&& entry)
        {
            if constexpr (toml::is_table<EntryType>)
            {
                const auto name = *entry["name"].template value<std::string>();
                const auto mass = *entry["mass"].template value<double>();
                const auto radius = *entry["radius"].template value<double>();
                const auto scale = *entry["scale"].template value<double>();
                const auto positionArray = entry["position"].as_array();
                Vector3 position{};
                auto* positionPtr = reinterpret_cast<float*>(&position);
                fillRaylibType(positionArray, positionPtr);

                const auto attitudeArray = entry["attitude"].as_array();
                Quaternion attitude{};
                auto* attitudePtr = reinterpret_cast<float*>(&attitude);
                fillRaylibType(attitudeArray, attitudePtr);

                const auto rotationSpeedArray = entry["rotationSpeed"].as_array();
                Vector3 rotationSpeed{};
                auto* rotationSpeedPtr = reinterpret_cast<float*>(&rotationSpeed);
                fillRaylibType(rotationSpeedArray, rotationSpeedPtr);

                /*const auto refBodyName = entry["refBody"].template value<std::string>();
                std::optional<ComponentInstance> refBody{std::nullopt};
                if (refBodyName.has_value())
                {
                    const auto refEntity = entityManager->getEntityByName(*refBodyName);
                    if (!refEntity.has_value())
                        throw std::runtime_error("Could not find ref body");

                    refBody = componentsRegister->getComponentInstance<Orbiting>(*refEntity);
                }*/

                const auto shaderName = entry["shaderName"].template value_or<std::string>("planet");
                const auto currentEntity = createBody(name, mass, radius * scale, position, attitude, rotationSpeed,
                                                      std::nullopt, shaderName);

                if (name == "Sun")
                {
                    componentsRegister->addComponent<Light>(currentEntity, SS3D::Light{
                                                                .handle = 0,
                                                                .type = LightType::POINT,
                                                                .color = WHITE,
                                                                .attenuation = 1000.f,
                                                            });
                }
            }
        });
    }

    void SolarSystem::setSystem(const std::filesystem::path& luaFile)
    {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        luaopen_math(L);
        luaopen_string(L);
    }

    Entity SolarSystem::createBody(const std::string& name, const float mass, const float radius,
                                   const Vector3& position,
                                   const Quaternion& attitude,
                                   const Vector3& rotationSpeed,
                                   const std::optional<ComponentInstance> refBody,
                                   const std::string& shaderName)
    {
        const auto bodyEntity = entityManager->createEntity(name, "body");
        bodies[name] = bodyEntity;

        componentsRegister->addComponent<Transform>(bodyEntity, SS3D::Transform{
                                                        .position = Vector3Scale(position, modelScale),
                                                        .rotation = attitude,
                                                        .scale = radius * modelScale,
                                                    });

        componentsRegister->addComponent<Motion>(bodyEntity, SS3D::Motion{
                                                     .velocity = Vector3{},
                                                     .rotationSpeed = rotationSpeed,
                                                 });

        Material material;
        makeMaterial(name, material, shaderName);
        const auto Model = LoadModel((resourcePath / "Planet.glb").c_str());


        componentsRegister->addComponent<Graphics>(bodyEntity, SS3D::Graphics{
                                                       .type = GraphicsType::MODEL,
                                                       .material = material,
                                                       .model = Model,
                                                   });

        componentsRegister->addComponent<Orbiting>(bodyEntity, SS3D::Orbiting{
                                                       .mass = mass,
                                                       .refBody = refBody,
                                                   });


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
        ecs.systemRegister->getSystem<ControlsSystem>()->render();
        ecs.systemRegister->getSystem<PhysicsSystem>()->render();
        ecs.systemRegister->getSystem<RenderingSystem>()->render();
        ecs.systemRegister->getSystem<LightingSystem>()->render();
        ecs.systemRegister->getSystem<MovementSystem>()->render();
    }

    void SolarSystem::makeMaterial(const std::string& bodyName, Material& material, const std::string& shaderName) const
    {
        const auto texturesPath = resourcePath / "SolarTextures";
        const auto diffuseTexture = LoadTexture((texturesPath / (bodyName + "_diffuse.jpg")).c_str());
        const auto specularTexture = LoadTexture((texturesPath / (bodyName + "_specular.png")).c_str());
        const auto normalMap = LoadTexture((texturesPath / (bodyName + "_normal.tif")).c_str());
        const auto nightTexture = LoadTexture((texturesPath / (bodyName + "_emit_night.jpg")).c_str());

        material = LoadMaterialDefault();
        material.shader = ecs.systemRegister->getSystem<RenderingSystem>()->renderer->getShader(shaderName);
        SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, diffuseTexture);
        SetMaterialTexture(&material, MATERIAL_MAP_SPECULAR, specularTexture);
        SetMaterialTexture(&material, MATERIAL_MAP_NORMAL, normalMap);
        SetMaterialTexture(&material, MATERIAL_MAP_EMISSION, nightTexture);

        // const auto nightLocation = GetShaderLocation(material.shader, "texture3");
        // SetShaderValueTexture(material.shader, nightLocation, nightTexture);
    }
}
