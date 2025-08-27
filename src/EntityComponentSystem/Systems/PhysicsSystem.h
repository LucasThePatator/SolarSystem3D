//
// Created by lucas on 6/28/25.
//

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <vector>

#include "System.h"
#include "../Components/Transform.h"

namespace SS3D
{
    class PhysicsSystem : public System
    {
    public:
        explicit PhysicsSystem();

        void initialize() override;
        void update(float dt) override;
        void render() override;
    };
}

#endif //PHYSICSSYSTEM_H