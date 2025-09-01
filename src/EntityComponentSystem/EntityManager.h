//
// Created by lucas on 6/29/25.
//

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <queue>
#include <array>
#include <optional>
#include <unordered_map>

#include "Systems/Systems.h"

namespace SS3D
{
    class SystemRegister;
    class ComponentsRegister;

    class EntityManager
    {
    public:
        EntityManager();

        Entity createEntity(const std::string& name,
                            const std::string& tag);

        void destroyEntity(Entity entity);
        void setSignature(Entity entity, Signature signature);

        [[nodiscard]] Signature getSignature(Entity entity) const;
        std::optional<Entity> getEntityByName(const std::string& name);
        std::optional<std::vector<Entity>> getEntitiesByTag(const std::string& tag);

        void setSystemRegister(const std::shared_ptr<SystemRegister>&);
        void setComponentRegister(const std::shared_ptr<ComponentsRegister>&);

    protected:
        std::queue<Entity> availableEntities{};
        std::array<Signature, MAX_ENTITIES> signatures{};
        std::unordered_map<std::string, Entity> namesToEntity{};
        std::unordered_map<Entity, std::string> entityToNames{};

        std::unordered_map<std::string, std::vector<Entity>> tagToEntities{};
        std::unordered_map<Entity, std::string> entityToTag{};

        std::shared_ptr<SystemRegister> systemRegister{};
        std::shared_ptr<ComponentsRegister> componentsRegister{};
    };
}

#endif //ENTITY_MANAGER_H
