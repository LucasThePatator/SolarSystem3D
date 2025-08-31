//
// Created by lucas on 7/24/25.
//

#include "SystemRegister.h"
#include "Systems/System.h"
#include <ranges>

namespace SS3D
{
    void SystemRegister::onEntitySignatureChanged(const Entity entity, const Signature entitySignature)
    {
        for (const auto& [systemName, system] : systems)
        {
            if (system->isEntityRegistered(entity))
                continue;

            if ((signatures.at(systemName) & entitySignature) == signatures.at(systemName))
            {
                system->registerEntity(entity);
            }
            else
            {
                system->unregisterEntity(entity);
            }
        }
    }

    void SystemRegister::onEntityDestroyed(const Entity entity)
    {
        for (const auto& system : systems | std::views::values)
        {
            system->unregisterEntity(entity);
        }
    }
}
