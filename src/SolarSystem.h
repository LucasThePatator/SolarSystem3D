//
// Created by lucas on 7/26/25.
//

#ifndef INC_3DSOLARSYSTEM_SOLARSYSTEM_H
#define INC_3DSOLARSYSTEM_SOLARSYSTEM_H

#include <unordered_map>
#include <optional>
#include <filesystem>

#include "EntityComponentSystem/EntityComponentSystem.h"
#include "EntityComponentSystem/Types.h"
#include "EntityComponentSystem/Components/Orbiting.h"

namespace SS3D
{
    class SolarSystem
    {
    public:
        SolarSystem() = delete;
        explicit SolarSystem(EntityComponentSystem& ecs, std::filesystem::path resourcePath);

        Entity createBody(const std::string& name, double mass, double radius, const Vector3& position,
                          const Quaternion& attitude, std::optional<ComponentInstance> refBody = std::nullopt);

        void update(float deltaTime);
        void render();

    private:
        std::unordered_map<std::string, Entity> bodies;

        EntityComponentSystem& ecs;
        std::shared_ptr<ComponentsRegister> componentsRegister;
        std::shared_ptr<EntityManager> entityManager;

        std::shared_ptr<RenderingSystem> renderingSystem;

        const std::filesystem::path resourcePath;

        void makeMaterial(const std::string &bodyName, Material &material) const;
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_SOLARSYSTEM_H
