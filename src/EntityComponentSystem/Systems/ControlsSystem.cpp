//
// Created by lucas on 8/20/25.
//

#include "ControlsSystem.h"

#include <raylib.h>
#include "src/EntityComponentSystem/ComponentsRegister.h"
#include "src/EntityComponentSystem/Components/Camera.h"
#include "src/Renderer/Renderer.h"

namespace SS3D
{
    ControlsSystem::ControlsSystem(const std::shared_ptr<SS3D::Renderer::Renderer>& renderer) :
    renderer(renderer)
    {
    }

    void ControlsSystem::initialize()
    {
        auto& [position, target, up, fovy, projection] = renderer->camera;

        position = {100.f, 10.f, 0.f};
        target = {0.0f, 0.0f, 0.0f};
        up = {0.0f, 0.0f, 1.0f};
        fovy = 45.0f;
        projection = CAMERA_PERSPECTIVE;
    }

    void ControlsSystem::update(float dt)
    {
        if (entities.empty())
        {
            throw std::runtime_error("At least one entity must be a Camera");
        }
        if (entities.size() > 1)
        {
            throw std::runtime_error("Only one entity can be controlled");
        }

        const auto cameraEntity = entities[0];
        auto &transform = componentsRegister->getComponent<Transform>(cameraEntity);
        auto& [cameraMode] = componentsRegister->getComponent<SS3D::Camera>(cameraEntity);

        auto& [position, target, up, fovy, projection] = renderer->camera;

        if (IsKeyPressed(KEY_ONE))
        {
            cameraMode = CAMERA_FREE;
            spdlog::info("Camera free");
        }
        if (IsKeyPressed(KEY_TWO))
        {
            cameraMode = CAMERA_ORBITAL;
            spdlog::info("Camera orbital");
        }
        if (IsKeyPressed(KEY_THREE))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            spdlog::info("Camera FP");
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            cameraMode = CAMERA_THIRD_PERSON;
            spdlog::info("Camera TP");
        }
        if (IsKeyPressed(KEY_R))
        {
            transform.position = Vector3{100.f, 10.f, 0.f};
            target = {0.0f, 0.0f, 0.0f};
            spdlog::info("Position reset");
        }

        position = transform.position;
        UpdateCamera(&renderer->camera, cameraMode);
        transform.position = position;
    }
} // SS3D