//
// Created by lucas on 6/28/25.
//

#ifndef GRAPHICAL_H
#define GRAPHICAL_H

#include <memory>
#include <raylib.h>
#include <unordered_map>
#include <variant>

namespace SS3D
{
    enum class GraphicsType
    {
        SPHERE,
        MODEL,
        ATMOSPHERE,
    };

    struct Graphics
    {
        GraphicsType type;
        Material material;
        Model model;
        std::unordered_map<std::string, std::variant<int, float, Vector3>> renderParameters;
    };
}


#endif //GRAPHICAL_H