//
// Created by lucas on 6/28/25.
//

#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <vector>

#include "../Types.h"

namespace SS3D
{
    class ComponentsRegister;
    class EntityManager;

    class System
    {

    public:
        System() = default;
        virtual ~System() = default;

        virtual void initialize() = 0;
        virtual void update(float dt) = 0;

        virtual void beforeRender()
        {
        };

        virtual void afterRender()
        {
        };
        virtual void render() = 0;

        void setComponentsRegister(const std::shared_ptr<ComponentsRegister>& reg)
        {
            componentsRegister = reg;
        }

        void setEntityManager(const std::shared_ptr<EntityManager>& manager)
        {
            entityManager = manager;
        }

        void registerEntity(Entity entity);
        void unregisterEntity(Entity entity);
        bool isEntityRegistered(Entity entity);

    protected:
        std::shared_ptr<ComponentsRegister> componentsRegister{};
        std::shared_ptr<EntityManager> entityManager{};
        std::vector<Entity> entities{};
    };
}


#endif //SYSTEM_H