//
// Created by lucas on 6/28/25.
//

#ifndef GRAPHICAL_H
#define GRAPHICAL_H

#include <memory>
#include <raylib.h>

namespace SS3D
{
    enum class GraphicsType
    {
        SPHERE,
        MODEL,
    };

    struct Graphics
    {
        GraphicsType type;
        Material material;
        Model model;
        Mesh mesh;
    };
}


#endif //GRAPHICAL_H
