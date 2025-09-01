//
// Created by lucas on 6/29/25.
//

#ifndef MOTION_H
#define MOTION_H

#include <raylib.h>


namespace SS3D
{
    struct Motion
    {
        Vector3 velocity;
        Vector3 rotationSpeed; //Rotation vector per second
    };
}


#endif //MOTION_H