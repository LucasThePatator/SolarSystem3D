//
// Created by lucas on 6/28/25.
//

#include <raylib.h>
#include "RenderingSystem.h"

#include "../EntityManager.h"
#include "../ComponentsRegister.h"

namespace SS3D
{
    RenderingSystem::RenderingSystem(const std::shared_ptr<SS3D::Renderer::Renderer> &renderer) :
        System(),
        renderer(renderer)
    {
    }

    void RenderingSystem::initialize()
    {
        System::initialize();
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
            const auto& [position, rotation, scale] = componentsRegister->getComponent<Transform>(entity);
            if (graphic.type == GraphicsType::MODEL)
            {
                renderer->renderMesh(graphic.model.meshes[0], graphic.material, position, rotation,
                                     scale);
            }
        }
        renderer->endRender();
    }
}
