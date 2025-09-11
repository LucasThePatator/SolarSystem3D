//
// Created by lucas on 8/20/25.
//

#include "ControlsSystem.h"

#include "src/EntityComponentSystem/ComponentsRegister.h"
#include "src/EntityComponentSystem/Components/Camera.h"
#include "src/EntityComponentSystem/Components/Transform.h"
#include "src/Renderer/Renderer.h"

#define RAYGUI_IMPLEMENTATION
#include "src/Renderer/raygui.h"

namespace SS3D
{
    struct Transform;

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
        auto& transform = componentsRegister->getComponent<SS3D::Transform>(cameraEntity);
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
            target = currentTarget;
            spdlog::info("Position reset");
        }

        position = transform.position;
        UpdateCamera(&renderer->camera, cameraMode);
        transform.position = position;
    }

    void ControlsSystem::render()
    {
        renderGui();
    }

    void ControlsSystem::renderGui()
    {
        const auto& bodyEntities = entityManager->getEntitiesByTag("body");
        std::vector<std::string> bodyNames;
        std::string bodyNamesString{};

        for (const auto& entity : *bodyEntities)
        {
            const std::string& bodyName = entityManager->getEntityName(entity);
            bodyNamesString += bodyName + ";";
            bodyNames.push_back(bodyName);
        }
        bodyNamesString.resize(!bodyNamesString.empty() ? bodyNamesString.size() - 1 : 0);
        GuiLabel(Rectangle{10, 10, 60, 24}, "Target body:");
        static bool bodySelectorEditMode = false;
        if (GuiDropdownBox(Rectangle{80, 10, 120, 24}, bodyNamesString.c_str(), &bodyIndex, bodySelectorEditMode))
            bodySelectorEditMode = !bodySelectorEditMode;

        if (!bodyNames.empty())
        {
            const auto& targetEntityName = bodyNames[bodyIndex];
            const auto& targetEntity = entityManager->getEntityByName(targetEntityName);
            const auto& targetEntityTransform = componentsRegister->getComponent<Transform>(*targetEntity);

            currentTarget = targetEntityTransform.position;
        }

        auto* const speedUp = static_cast<float*>(parameterPointers.at("speedUp"));
        GuiSlider(Rectangle{12, 44, 120, 24}, "Speed up", std::format("x{:0.1f}", *speedUp).c_str(), speedUp, 1,
                  6);
    }
} // SS3D
