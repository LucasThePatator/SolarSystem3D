//
// Created by lucas on 6/28/25.
//

#ifndef COMPONENTCOLLECTION_H
#define COMPONENTCOLLECTION_H

#include <map>
#include <stdexcept>
#include "Types.h"

namespace SS3D
{
    class AbstractComponentCollection
    {
    public:
        virtual ~AbstractComponentCollection() = default;
        virtual void onEntityDestroyed(Entity entity) = 0;
    };

    template <typename ComponentType>
    class ComponentCollection : public AbstractComponentCollection
    {
    public:
        ComponentInstance addComponent(const Entity e, ComponentType&& component)
        {
            ComponentInstance instance = size;
            data[instance] = std::move(component);
            instances[e] = instance;
            ++size;

            return instance;
        }

        ComponentInstance addComponent(const Entity e, ComponentInstance instance)
        {
            instances[e] = instance;
            return instance;
        }

        void removeComponent(const Entity e)
        {
            //Keep things packed
            const auto removedComponentInstance = instances.at(e);
            const auto previousLastEntity = getEntity(size - 1);
            data[removedComponentInstance] = std::move(data[size - 1]);
            --size;

            instances[previousLastEntity] = removedComponentInstance;
            instances.erase(e);
        }

        [[nodiscard]] ComponentType& getComponent(const Entity e)
        {
            const auto componentInstance = instances.at(e);
            return data.at(componentInstance);
        }

        [[nodiscard]] ComponentInstance getComponentInstance(const Entity e) const
        {
            return instances.at(e);
        }

        [[nodiscard]] Entity getEntity(const ComponentInstance instance)
        {
            for (const auto& [entity, localInstance] : instances)
            {
                if (instance == localInstance)
                {
                    return entity;
                }
            }
            throw std::runtime_error("Entity does not exist");
        }

        void onEntityDestroyed(const Entity entity) override
        {
            removeComponent(entity);
        }

    private:
        size_t size{0};
        std::array<ComponentType, MAX_ENTITIES> data;
        std::map<Entity, ComponentInstance> instances;
        std::map<ComponentInstance, Entity> instancesToEntity;
    };
}

#endif //COMPONENTCOLLECTION_H