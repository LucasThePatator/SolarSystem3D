//
// Created by lucas on 6/28/25.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <raylib.h>

namespace SS3D
{
    struct Transform
    {
        Vector3 position;
        Quaternion rotation;
        float scale;
    };
}

#endif //TRANSFORM_H