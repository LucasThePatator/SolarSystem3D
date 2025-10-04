//
// Created by lucas on 6/28/25.
//

#include <raylib.h>
#include "RenderingSystem.h"

#include <raymath.h>
#include <rlgl.h>

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

    void RenderingSystem::update(const float dt)
    {
        renderer->setTime(dt);
    }

    void RenderingSystem::render()
    {
        BeginMode3D(renderer->camera); //TODO très bof
        renderer->renderSkybox();

        double near = std::numeric_limits<double>::max();
        double far = 0;
        for (const auto& entity : entities)
        {
            const auto& [position, rotation, scale] = componentsRegister->getComponent<SS3D::Transform>(entity);

            const double distance = Vector3Distance(position, renderer->camera.position);
            near = std::min(near, distance);
            far = std::max(far, distance);
        }

        //rlSetClipPlanes(near / 2, far * 10);

        for (const auto& entity : entities)
        {
            const auto& graphic = componentsRegister->getComponent<Graphics>(entity);
            const auto& [position, rotation, scale] = componentsRegister->getComponent<SS3D::Transform>(entity);
            if (graphic.type == GraphicsType::SPHERE)
            {
                renderer->renderMesh(graphic.model.meshes[0], graphic.material, position, rotation,
                                     scale, graphic.renderParameters);
            }
            else if (graphic.type == GraphicsType::MODEL)
            {
                renderer->renderModel(graphic.model, position, rotation,
                                      scale, graphic.renderParameters);
            }
            else if (graphic.type == GraphicsType::ATMOSPHERE)
            {
                renderer->renderAtmosphere(graphic.model.meshes[0], graphic.material, position, rotation,
                                           scale, graphic.renderParameters);
            }
        }
        EndMode3D();
    }

    void RenderingSystem::beforeRender()
    {
        renderer->startRender();
    }

    void RenderingSystem::afterRender()
    {
        renderer->endRender();
    }
}
