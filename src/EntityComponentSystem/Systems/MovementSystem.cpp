//
// Created by lucas on 6/29/25.
//
#include <raymath.h>

#include "MovementSystem.h"

#include "src/EntityComponentSystem/ComponentsRegister.h"
#include "src/EntityComponentSystem/Components/Motion.h"

namespace SS3D
{
    void MovementSystem::initialize()
    {
    }

    void MovementSystem::update(const float dt)
    {
        for (const auto entity : entities)
        {
            auto &transform = componentsRegister->getComponent<Transform>(entity);
            auto & [velocity, rotationSpeed] = componentsRegister->getComponent<Motion>(entity);

            transform.position += velocity * dt;
            const auto angle = Vector3Length(rotationSpeed);
            const auto axis = Vector3Normalize(rotationSpeed);
            const auto AddedRotation = QuaternionFromAxisAngle(axis, angle * dt);
            transform.rotation = QuaternionMultiply(AddedRotation, transform.rotation);
        }
    }

    MovementSystem::MovementSystem() : System()
    {
    }
}
