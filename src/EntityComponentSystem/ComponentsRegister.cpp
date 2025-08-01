//
// Created by lucas on 7/25/25.
//

#include "ComponentsRegister.h"
#include <ranges>

#include "EntityManager.h"

namespace SS3D
{
    ComponentsRegister::ComponentsRegister()
    {
    }

    void ComponentsRegister::onEntityDestroyed(const Entity entity)
    {
        for (const auto& collection : componentCollections | std::views::values)
        {
            collection->onEntityDestroyed(entity);
        }
    }

    void ComponentsRegister::setEntityManager(const std::shared_ptr<EntityManager>& em)
    {
        entityManager = em;
    }
}