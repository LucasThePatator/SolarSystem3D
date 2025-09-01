//
// Created by lucas on 6/28/25.
//

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <vector>

#include "System.h"
#include "../../NBodySimulation/BarnesHut.h"

namespace SS3D
{
    class PhysicsSystem : public System
    {
    public:
        explicit PhysicsSystem(float theta, float G, float scale);

        void initialize() override;
        void update(float dt) override;
        void render() override;

    private:
        BarnesHut barnesHut;
    };
}

#endif //PHYSICSSYSTEM_H