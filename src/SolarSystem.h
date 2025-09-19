//
// Created by lucas on 7/26/25.
//

#ifndef INC_3DSOLARSYSTEM_SOLARSYSTEM_H
#define INC_3DSOLARSYSTEM_SOLARSYSTEM_H

#include <raylib.h>
#include <toml++/toml.hpp>
#include <lua.hpp>

#include <filesystem>
#include <optional>
#include <unordered_map>

#include "Config.h"
#include "EntityComponentSystem/EntityComponentSystem.h"
#include "EntityComponentSystem/Types.h"

namespace SS3D
{
    class SolarSystem
    {
    public:
        SolarSystem() = delete;
        explicit SolarSystem(EntityComponentSystem& ecs, const std::filesystem::path& configurationFile);

        Entity createBody(const BodySpawnConfig& config
        );

        void update(float deltaTime);
        void render();
        void run();

    private:
        float modelScale{1.f};

        std::unordered_map<std::string, Entity> bodies;
        EntityComponentSystem& ecs;
        std::shared_ptr<ComponentsRegister> componentsRegister;
        std::shared_ptr<EntityManager> entityManager;

        std::filesystem::path resourcePath;
        void setSystem(const std::vector<BodySpawnConfig>& config);
        void makeMaterial(const std::string& bodyName, Material& material, const std::string& shaderName) const;
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_SOLARSYSTEM_H
