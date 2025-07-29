//
// Created by lucas on 6/28/25.
//

#include <raylib.h>
#include "RenderingSystem.h"

#include "../EntityManager.h"
#include "../ComponentsRegister.h"

namespace SS3D
{
    RenderingSystem::RenderingSystem(const std::filesystem::path& shadersPath) : System(),
        width(1024),
        height(768), camera(),
        shadersPath(shadersPath)
    {
    }

    void RenderingSystem::initialize()
    {
        System::initialize();

        SetConfigFlags(FLAG_MSAA_4X_HINT);
        InitWindow(width, height, "SolarSystem 3D");

        camera.position = {20.0f, 20.0f, 20.0f}; // Camera position
        camera.target = {0.0f, 0.0f, 0.0f}; // Camera looking at point
        camera.up = {0.0f, 0.0f, 1.0f}; // Camera up vector (rotation towards target)
        camera.fovy = 45.0f; // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE; // Camera mode type

        SetTargetFPS(60); // Set our game to run at 60 frames-per-second

        for (const auto& dir_entry : std::filesystem::directory_iterator(shadersPath))
        {
            if (dir_entry.is_regular_file() && dir_entry.path().extension() == ".fs")
            {
                const auto& fragmentShaderPath = dir_entry.path();
                auto vertexShaderPath = dir_entry.path();
                vertexShaderPath.replace_extension(".vs");

                const Shader shader = LoadShader(vertexShaderPath.string().c_str(),
                                                 fragmentShaderPath.string().c_str());

                shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
                shaders[fragmentShaderPath.stem().string()] = shader;

                sunLight = CreateLight(LightType::POINT, {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                WHITE, shader, 0
                );
            }
        }
    }

    void RenderingSystem::update(float dt)
    {
    }

    void RenderingSystem::render()
    {
        //UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        for (const auto& entity : entities)
        {
            const auto& [type, material, model, mesh] = componentsRegister->getComponent<Graphics>(entity);
            const auto& [position, rotation, scale] = componentsRegister->getComponent<Transform>(entity);

            const auto matrix = makeTransformationMatrix(position, rotation, scale);

            if (type == GraphicsType::SPHERE)
            {
                DrawMesh(mesh, material, matrix);
            }
            else if (type == GraphicsType::MODEL)
            {
                DrawMesh(model.meshes[0], material, matrix);
            }
        }
        EndMode3D();
        EndDrawing();
    }

    Matrix RenderingSystem::makeTransformationMatrix(const Vector3& position, const Quaternion& rotation,
                                                     const float scale)
    {
        const Matrix T = MatrixTranslate(position.x, position.y, position.z);
        const Matrix R = QuaternionToMatrix(rotation);
        const Matrix S = MatrixScale(scale, scale, scale);

        const Matrix SR = MatrixMultiply(S, R);
        const Matrix TSR = MatrixMultiply(T, SR);
        return TSR;
    }

    Light RenderingSystem::CreateLight(const LightType type, const Vector3& position, const Vector3& target,
        const Color& color, const Shader& shader, const uint8_t lightIndex)
    {
        Light light = {
            .type = type,
            .enabled = true,
            .position = position,
            .target = target,
            .color = color,
        };

        // NOTE: Lighting shader naming must be the provided ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightIndex));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightIndex));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightIndex));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightIndex));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightIndex));

        // Send to shader light enabled state and type
        SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
        SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

        // Send to shader light position values
        const float position_data[3] = { light.position.x, light.position.y, light.position.z };
        SetShaderValue(shader, light.positionLoc, position_data, SHADER_UNIFORM_VEC3);

        // Send to shader light target position values
        const float target_data[3] = { light.target.x, light.target.y, light.target.z };
        SetShaderValue(shader, light.targetLoc, target_data, SHADER_UNIFORM_VEC3);

        // Send to shader light color values
        const float color_data[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255,
                           (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
        SetShaderValue(shader, light.colorLoc, color_data, SHADER_UNIFORM_VEC4);


        return light;
    }
}
