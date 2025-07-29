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

    class System
    {
    public:
        System() = default;
        virtual ~System() = default;

        virtual void initialize()
        {
        };

        virtual void update(float dt)
        {
        };

        virtual void render()
        {
        };

        void setComponentsRegister(const std::shared_ptr<ComponentsRegister>& reg)
        {
            componentsRegister = reg;
        }

        void registerEntity(Entity entity);
        void unregisterEntity(Entity entity);

    protected:
        std::shared_ptr<ComponentsRegister> componentsRegister{};
        std::vector<Entity> entities{};
    };
}


#endif //SYSTEM_H
