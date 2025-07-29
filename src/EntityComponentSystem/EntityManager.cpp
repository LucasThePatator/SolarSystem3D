//
// Created by lucas on 6/29/25.
//

#include "EntityManager.h"
#include "SystemRegister.h"
#include "ComponentsRegister.h"

namespace SS3D
{
    EntityManager::EntityManager()
    {
        for (Entity i = 0; i < MAX_ENTITIES; ++i)
        {
            availableEntities.push(i);
        }
    }

    Entity EntityManager::createEntity()
    {
        if (availableEntities.empty())
            throw std::runtime_error("No available entities");

        const auto entity = availableEntities.front();
        availableEntities.pop();
        return entity;
    }

    void EntityManager::destroyEntity(const Entity entity)
    {
        if (entity > MAX_ENTITIES)
            throw std::runtime_error("Entity out of range");

        availableEntities.push(entity);
        signatures.at(entity).reset();

        systemRegister->onEntityDestroyed(entity);
        componentsRegister->onEntityDestroyed(entity);
    }

    void EntityManager::setSignature(const Entity entity, const Signature signature)
    {
        if (entity > MAX_ENTITIES)
            throw std::runtime_error("Entity out of range");

        signatures[entity] = signature;
        systemRegister->onEntitySignatureChanged(entity, signature);
    }

    Signature EntityManager::getSignature(const Entity entity) const
    {
        if (entity > MAX_ENTITIES)
            throw std::runtime_error("Entity out of range");

        return signatures.at(entity);
    }

    void EntityManager::setSystemRegister(const std::shared_ptr<SystemRegister>& sr)
    {
        systemRegister = sr;
    }

    void EntityManager::setComponentRegister(const std::shared_ptr<ComponentsRegister>& cr)
    {
        componentsRegister = cr;
    }
}
