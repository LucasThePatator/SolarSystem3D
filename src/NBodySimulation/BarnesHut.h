//
// Created by patator on 31/08/2025.
//

#ifndef INC_3DSOLARSYSTEM_BARNESHUT_H
#define INC_3DSOLARSYSTEM_BARNESHUT_H

#include <vector>

struct Vector3;

namespace SS3D
{
    class BarnesHut
    {
    public:
        void computeAcceleration(const std::vector<Vector3>& positions, const std::vector<float>& mass,
                                 std::vector<Vector3>&
                                 acceleration) const;

        float theta = 0.1;
        float G = 6.6743e-11; //Gravitational constant
        float scale = 1.f;
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_BARNESHUT_H