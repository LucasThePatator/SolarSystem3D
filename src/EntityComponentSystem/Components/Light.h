//
// Created by lucas on 7/30/25.
//

#ifndef INC_3DSOLARSYSTEM_LIGHT_H
#define INC_3DSOLARSYSTEM_LIGHT_H

#include <memory>

#include "src/Renderer/Renderer.h"

namespace SS3D
{
    enum class LightType : int
    {
        DIRECTIONAL = 0,
        POINT
    };

    struct Light
    {
        Renderer::LightHandle handle{-1};
        LightType type{};
        Color color{WHITE};
        float attenuation{1.f};
        Vector3 target{};
    };
};

#endif //INC_3DSOLARSYSTEM_LIGHT_H