//
// Created by lucas on 6/28/25.
//

#include "System.h"

#include <algorithm>

namespace SS3D
{
    void System::registerEntity(const Entity entity)
    {
        entities.push_back(entity);
    }

    void System::unregisterEntity(const Entity entity)
    {
        for (auto it = entities.cbegin(); it != entities.cend(); ++it)
        {
            if (*it == entity)
            {
                entities.erase(it);
                return;
            }
        }
    }

    bool System::isEntityRegistered(const Entity entity)
    {
        {
            return std::ranges::find(entities, entity) != entities.cend();
        }
    }
}