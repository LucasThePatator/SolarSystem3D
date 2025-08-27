//
// Created by lucas on 6/28/25.
//

#include "System.h"

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
        return;
    }
}
