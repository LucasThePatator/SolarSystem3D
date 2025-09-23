//
// Created by lucas on 6/28/25.
//

#include <ranges>
#include "PhysicsSystem.h"

#include "src/EntityComponentSystem/ComponentsRegister.h"
#include "src/EntityComponentSystem/Components/Motion.h"
#include "src/EntityComponentSystem/Components/Orbiting.h"
#include "src/EntityComponentSystem/Components/Transform.h"

#include <raymath.h>

namespace SS3D
{
    PhysicsSystem::PhysicsSystem(const float theta, const float G, const float scale) :
        barnesHut{.theta = theta, .G = G, .scale = scale}
    {
        barnesHut.G *= scale * scale * scale;
    }

    void PhysicsSystem::initialize()
    {
    }

    void PhysicsSystem::update(const float dt)
    {
        std::vector<Vector3> positions;
        std::vector<float> masses;
        std::vector<Vector3> accelerations;

        for (const auto& entity : entities)
        {
            const auto& [position, rotation, scale] = componentsRegister->getComponent<SS3D::Transform>(entity);
            const auto& [mass, refBody] = componentsRegister->getComponent<Orbiting>(entity);

            positions.push_back(position);
            masses.push_back(mass);
        }

        barnesHut.computeAcceleration(positions, masses, accelerations);

        for (const auto& [entity, acc] : std::views::zip(entities, accelerations))
        {
            auto& [velocity, rotationSpeed] = componentsRegister->getComponent<Motion>(entity);
            velocity = Vector3Add(velocity, Vector3Scale(acc, dt));
        }
    }

    void PhysicsSystem::render()
    {
    }
}
