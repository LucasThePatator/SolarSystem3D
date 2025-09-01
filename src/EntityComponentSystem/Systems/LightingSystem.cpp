//
// Created by lucas on 7/31/25.
//

#include "LightingSystem.h"
#include "src/Renderer/Renderer.h"
#include "../ComponentsRegister.h"
#include "src/EntityComponentSystem/Components/Light.h"
#include "src/EntityComponentSystem/Components/Transform.h"

namespace SS3D
{
    LightingSystem::LightingSystem(const std::shared_ptr<SS3D::Renderer::Renderer>& renderer)
        : renderer(renderer)
    {
    }

    void LightingSystem::initialize()
    {
    }

    void LightingSystem::update(const float dt)
    {
        for (const auto& entity : entities)
        {
            const auto& [position, rotation, scale] = componentsRegister->getComponent<SS3D::Transform>(entity);
            const auto& light = componentsRegister->getComponent<Light>(entity);
            renderer->updateLight(light.handle, position, light.target, light.color, true);
        }
    }

    void LightingSystem::render()
    {
    }
} // SS3D