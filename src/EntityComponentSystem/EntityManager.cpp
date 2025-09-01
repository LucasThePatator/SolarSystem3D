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

    Entity EntityManager::createEntity(const std::string& name, const std::string& tag)
    {
        if (availableEntities.empty())
            throw std::runtime_error("No available entities");

        if (namesToEntity.contains(name))
        {
            const auto& existingEntityId = getEntityByName(name);
            throw std::runtime_error("Entity already exists with that name");
        }

        const auto entity = availableEntities.front();
        namesToEntity[name] = entity;
        entityToNames[entity] = name;

        tagToEntities[tag].push_back(entity);
        entityToTag[entity] = tag;

        availableEntities.pop();
        return entity;
    }

    void EntityManager::destroyEntity(const Entity entity)
    {
        if (entity > MAX_ENTITIES)
            throw std::runtime_error("Entity out of range");

        //Clean up the names and tag vectors
        const auto& entityName = entityToNames.at(entity);
        namesToEntity.erase(entityName);
        entityToNames.erase(entity);

        const auto& entityTag = entityToTag.at(entity);
        auto& taggedEntities = tagToEntities.at(entityTag);
        taggedEntities.erase(std::ranges::find(taggedEntities, entity));
        if (taggedEntities.empty())
            tagToEntities.erase(entityTag);
        entityToTag.erase(entity);

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

    [[nodiscard]] Signature EntityManager::getSignature(const Entity entity) const
    {
        if (entity > MAX_ENTITIES)
            throw std::runtime_error("Entity out of range");

        return signatures.at(entity);
    }

    [[nodiscard]] std::optional<Entity> EntityManager::getEntityByName(const std::string& name)
    {
        if (namesToEntity.contains(name))
            return namesToEntity.at(name);

        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::vector<Entity>> EntityManager::getEntitiesByTag(const std::string& tag)
    {
        if (tagToEntities.contains(tag))
            return std::optional<std::vector<Entity>>{namesToEntity.at(tag)};

        return std::nullopt;
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
