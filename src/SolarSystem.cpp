//
// Created by lucas on 7/26/25.
//

#include <spdlog/spdlog.h>

#include "SolarSystem.h"
#include "EntityComponentSystem/Components/Components.h"
#include "Renderer/Renderer.h"
namespace SS3D
{
    SolarSystem::SolarSystem(EntityComponentSystem& ecs, std::filesystem::path resourcePath) :
        ecs(ecs),
        componentsRegister(ecs.componentsRegister),
        entityManager(ecs.entityManager),
        resourcePath(std::move(resourcePath))
    {
        ecs.componentsRegister->registerComponentType<SS3D::Transform>();
        ecs.componentsRegister->registerComponentType<Motion>();
        ecs.componentsRegister->registerComponentType<Graphics>();
        ecs.componentsRegister->registerComponentType<Orbiting>();

        const auto renderer = std::make_shared<Renderer::Renderer>(1024, 768);
        renderer->initialize("/home/lucas/workspace/3DSolarSystem/src/shaders");
        renderingSystem = ecs.systemRegister->registerSystem<RenderingSystem>(Signature("00000101"), renderer);
        renderingSystem->initialize();

        spdlog::info("Solar System Initialized");
    }

    Entity SolarSystem::createBody(const std::string& name, const double mass, const double radius,
                                   const Vector3& position,
                                   const Quaternion& attitude, const std::optional<ComponentInstance> refBody)
    {
        const auto bodyEntity = entityManager->createEntity();
        bodies[name] = bodyEntity;

        componentsRegister->addComponent<Transform>(bodyEntity, SS3D::Transform{
                                                        .position = position,
                                                        .rotation = attitude,
                                                        .scale = static_cast<float>(radius) / 1000,
                                                    });

        componentsRegister->addComponent<Motion>(bodyEntity, SS3D::Motion{
                                                     .velocity = Vector3{},
                                                     .rotationSpeed = Vector3{},
                                                 });

        Material material;
        makeMaterial(name, material);
        const auto Model = LoadModel((resourcePath / "Planet.glb").c_str());

        const auto sphere = GenMeshSphere(1, 10, 10);


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
        renderingSystem->update(deltaTime);
    }

    void SolarSystem::render()
    {
        renderingSystem->render();
    }

    void SolarSystem::makeMaterial(const std::string& bodyName, Material& material) const
    {
        const auto texturesPath = resourcePath / "SolarTextures";
        const auto diffuseTexture = LoadTexture((texturesPath / (bodyName + "_diffuse.jpg")).c_str());
        const auto specularTexture = LoadTexture((texturesPath / (bodyName + "_specular.tif")).c_str());
        const auto normalMap = LoadTexture((texturesPath / (bodyName + "_normal.tif")).c_str());
        material = LoadMaterialDefault();
        SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, diffuseTexture);
        //SetMaterialTexture(&material, MATERIAL_MAP_SPECULAR, specularTexture);
        // SetMaterialTexture(&material, MATERIAL_MAP_NORMAL, normalMap);
    }
}
