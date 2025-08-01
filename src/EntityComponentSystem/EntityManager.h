//
// Created by lucas on 6/29/25.
//

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <queue>
#include <array>
#include "Systems/Systems.h"

namespace SS3D
{
    class SystemRegister;
    class ComponentsRegister;

    class EntityManager
    {
    public:
        EntityManager();

        Entity createEntity();
        void destroyEntity(Entity entity);
        void setSignature(Entity entity, Signature signature);
        [[nodiscard]] Signature getSignature(Entity entity) const;

        void setSystemRegister(const std::shared_ptr<SystemRegister>&);
        void setComponentRegister(const std::shared_ptr<ComponentsRegister>&);

    protected:
        std::queue<Entity> availableEntities{};
        std::array<Signature, MAX_ENTITIES> signatures{};

        std::shared_ptr<SystemRegister> systemRegister{};
        std::shared_ptr<ComponentsRegister> componentsRegister{};
    };
}

#endif //ENTITY_MANAGER_H