//
// Created by lucas on 6/28/25.
//

#include <ranges>
#include "PhysicsSystem.h"

#include "src/EntityComponentSystem/ComponentsRegister.h"
#include "src/EntityComponentSystem/Components/Motion.h"
#include "src/EntityComponentSystem/Components/Orbiting.h"

namespace SS3D
{
    PhysicsSystem::PhysicsSystem(const float theta, const float G) :
        barnesHut{.theta = theta, .G = G}
    {
    }

    void PhysicsSystem::initialize()
    {
    }

    void PhysicsSystem::update(float dt)
    {
        std::vector<Vector3> positions;
        std::vector<float> masses;
        std::vector<Vector3> accelerations;

        for (const auto& entity : entities)
        {
            const auto& [position, rotation, scale] = componentsRegister->getComponent<Transform>(entity);
            const auto& [mass, refBody] = componentsRegister->getComponent<Orbiting>(entity);

            positions.push_back(position);
            masses.push_back(mass);
        }

        barnesHut.computeAcceleration(positions, masses, accelerations);

        for (const auto& [entity, acc] : std::views::zip(entities, accelerations))
        {
            auto& [velocity, rotationSpeed] = componentsRegister->getComponent<Motion>(entity);
            velocity = Vector3Add(velocity, Vector3Scale(acc, dt)); //TODO rescale ! And very probably use doubles
        }
    }

    void PhysicsSystem::render()
    {
    }
}
