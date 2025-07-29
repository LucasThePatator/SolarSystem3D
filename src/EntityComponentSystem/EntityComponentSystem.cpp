//
// Created by lucas on 7/25/25.
//

#include "EntityComponentSystem.h"

namespace SS3D
{
    EntityComponentSystem::EntityComponentSystem()
    {
        entityManager->setComponentRegister(componentsRegister);
        entityManager->setSystemRegister(systemRegister);

        componentsRegister->setEntityManager(entityManager);

        systemRegister->setComponentsRegister(componentsRegister);
    }
} // SS3D
