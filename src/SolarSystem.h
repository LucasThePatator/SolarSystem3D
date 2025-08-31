//
// Created by lucas on 7/26/25.
//

#ifndef INC_3DSOLARSYSTEM_SOLARSYSTEM_H
#define INC_3DSOLARSYSTEM_SOLARSYSTEM_H

#include <raylib.h>
#include <toml++/toml.hpp>

#include <filesystem>
#include <optional>
#include <unordered_map>

#include "EntityComponentSystem/EntityComponentSystem.h"
#include "EntityComponentSystem/Types.h"

namespace SS3D
{
    class SolarSystem
    {
    public:
        SolarSystem() = delete;
        explicit SolarSystem(EntityComponentSystem& ecs, const std::filesystem::path& configurationFile);

        Entity createBody(const std::string& name, float mass, float radius, const Vector3& position,
                          const Quaternion& attitude,
                          const Vector3& rotationSpeed = Vector3{},
                          std::optional<ComponentInstance> refBody = std::nullopt,
                          const std::string& shaderName = "planet");

        void update(float deltaTime);
        void render();

    private:
        std::unordered_map<std::string, Entity> bodies;

        EntityComponentSystem& ecs;
        std::shared_ptr<ComponentsRegister> componentsRegister;
        std::shared_ptr<EntityManager> entityManager;

        std::filesystem::path resourcePath;
        void setSystem(const toml::table& tbl);

        void makeMaterial(const std::string& bodyName, Material& material, const std::string& shaderName) const;
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_SOLARSYSTEM_H