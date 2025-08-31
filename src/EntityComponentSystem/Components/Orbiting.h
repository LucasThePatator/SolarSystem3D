//
// Created by lucas on 6/29/25.
//

#ifndef ORBITING_H
#define ORBITING_H

#include <optional>

namespace SS3D
{
    struct Orbiting
    {
        float mass = 1.f;
        std::optional<ComponentInstance> refBody{};
    };
}


#endif //ORBITING_H