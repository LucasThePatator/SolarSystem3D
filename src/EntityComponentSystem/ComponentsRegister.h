//
// Created by lucas on 7/25/25.
//

#ifndef INC_3DSOLARSYSTEM_COMPONENTSREGISTER_H
#define INC_3DSOLARSYSTEM_COMPONENTSREGISTER_H
#include <memory>
#include <unordered_map>
#include <spdlog/spdlog.h>

#include "Types.h"
#include "ComponentCollection.h"
#include "EntityManager.h"

namespace SS3D
{
    class ComponentsRegister
    {
    public:
        ComponentsRegister();

        template <typename ComponentType>
        void registerComponentType()
        {
            const auto componentName = typeid(ComponentType).name();
            const auto componentTypeId = nextComponentType++;

            componentTypes[componentName] = componentTypeId;
            componentCollections[componentName] = std::make_shared<ComponentCollection<ComponentType>>();

            spdlog::debug("Registered {}", componentName);

        }

        template <typename ComponentType>
        void addComponent(const Entity entity, ComponentType&& component)
        {
            auto collection = getComponentCollection<ComponentType>();
            collection->addComponent(entity, std::forward<ComponentType>(component));

            updateSignature<ComponentType>(entity, true);
        }

        template <typename ComponentType>
        void removeComponent(const Entity entity)
        {
            auto collection = getComponentCollection<ComponentType>();
            collection->removeComponent(entity);

            updateSignature<ComponentType>(entity, false);
        }

        template <typename ComponentType>
        ComponentType& getComponent(const Entity entity)
        {
            const auto collection = getComponentCollection<ComponentType>();
            return collection->getComponent(entity);
        }

        void onEntityDestroyed(Entity entity);
        void setEntityManager(const std::shared_ptr<EntityManager>&);

    protected:
        std::unordered_map<const char*, ComponentTypeID> componentTypes;
        std::unordered_map<const char*, std::shared_ptr<AbstractComponentCollection>> componentCollections;

        ComponentTypeID nextComponentType{0};

        std::shared_ptr<EntityManager> entityManager{};

        template <typename ComponentType>
        std::shared_ptr<ComponentCollection<ComponentType>> getComponentCollection()
        {
            const auto* componentName = typeid(ComponentType).name();
            const auto abstractCollection = componentCollections.at(componentName);
            return std::static_pointer_cast<ComponentCollection<ComponentType>>(abstractCollection);
        }

        template <typename ComponentType>
        void updateSignature(const Entity entity, const bool value)
        {
            auto entitySignature = entityManager->getSignature(entity);
            const auto sigIndex = componentTypes.at(typeid(ComponentType).name());
            entitySignature[sigIndex] = value;

            entityManager->setSignature(entity, entitySignature);
        }
    };
}


#endif //INC_3DSOLARSYSTEM_COMPONENTSREGISTER_H
