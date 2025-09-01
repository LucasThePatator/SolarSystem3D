//
// Created by lucas on 7/25/25.
//

#ifndef INC_3DSOLARSYSTEM_ENTITYCOMPONENTSYSTEM_H
#define INC_3DSOLARSYSTEM_ENTITYCOMPONENTSYSTEM_H
#include <memory>

#include "ComponentsRegister.h"
#include "EntityManager.h"
#include "SystemRegister.h"

namespace SS3D
{
    class EntityComponentSystem
    {
    public:
        EntityComponentSystem();

        const std::shared_ptr<EntityManager> entityManager{new EntityManager};
        const std::shared_ptr<SystemRegister> systemRegister{new SystemRegister};
        const std::shared_ptr<ComponentsRegister> componentsRegister{new ComponentsRegister};
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_ENTITYCOMPONENTSYSTEM_H