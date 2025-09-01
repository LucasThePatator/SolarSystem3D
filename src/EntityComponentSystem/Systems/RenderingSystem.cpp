//
// Created by lucas on 6/28/25.
//

#include <raylib.h>
#include "RenderingSystem.h"

#include "../EntityManager.h"
#include "../ComponentsRegister.h"
#include "src/Renderer/Renderer.h"
#include "../Components/Graphics.h"
#include "../Components/Transform.h"

namespace SS3D
{
    RenderingSystem::RenderingSystem(const std::shared_ptr<SS3D::Renderer::Renderer>& renderer) :
        System(),
        renderer(renderer)
    {
    }

    void RenderingSystem::initialize()
    {
    }

    void RenderingSystem::update(float dt)
    {
    }

    void RenderingSystem::render()
    {
        renderer->startRender();
        for (const auto& entity : entities)
        {
            const auto& graphic = componentsRegister->getComponent<Graphics>(entity);
            const auto& [position, rotation, scale] = componentsRegister->getComponent<SS3D::Transform>(entity);
            if (graphic.type == GraphicsType::MODEL)
            {
                renderer->renderMesh(graphic.model.meshes[0], graphic.material, position, rotation,
                                     scale);
            }
        }
        renderer->endRender();
    }
}