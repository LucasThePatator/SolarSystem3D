//
// Created by lucas on 7/30/25.
//

#include "Renderer.h"

#include <ranges>
#include <raymath.h>

#include "src/EntityComponentSystem/Components/Graphics.h"

namespace SS3D
{
    namespace Renderer
    {
        Renderer::Renderer(const int width, const int height) :
            width(width), height(height)
        {
        }

        void Renderer::initialize(const std::filesystem::path& shadersPath)
        {
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
                }
            }

            setupLightShaderInformation();
        }

        void Renderer::setupLightShaderInformation()
        {
            for (const auto& [shaderName, shader] : shaders)
            {
                for (LightHandle h = 0; h < MAX_LIGHTS; ++h)
                {
                    lightsShaderInformation[shaderName][h].enabledLoc = GetShaderLocation(
                        shader, TextFormat("lights[%i].enabled", h));
                    lightsShaderInformation[shaderName][h].typeLoc = GetShaderLocation(
                        shader, TextFormat("lights[%i].type", h));
                    lightsShaderInformation[shaderName][h].positionLoc = GetShaderLocation(
                        shader, TextFormat("lights[%i].position", h));
                    lightsShaderInformation[shaderName][h].targetLoc = GetShaderLocation(
                        shader, TextFormat("lights[%i].target", h));
                    lightsShaderInformation[shaderName][h].colorLoc = GetShaderLocation(
                        shader, TextFormat("lights[%i].color", h));
                }
            }
        }

        Matrix Renderer::makeTransformationMatrix(const Vector3& position, const Quaternion& rotation,
                                                  const float scale)
        {
            const Matrix T = MatrixTranslate(position.x, position.y, position.z);
            const Matrix R = QuaternionToMatrix(rotation);
            const Matrix S = MatrixScale(scale, scale, scale);

            const Matrix SR = MatrixMultiply(S, R);
            const Matrix TSR = MatrixMultiply(T, SR);
            return TSR;
        }

        void Renderer::updateLight(const LightHandle id, const Vector3& position, const Vector3& target,
                                   const Color& color, const bool enabled)
        {
            for (const auto& [shaderName, shader] : shaders)
            {
                const auto &lightShaderInfo = lightsShaderInformation[shaderName][id];
                // Send to shader light enabled state and type
                SetShaderValue(shader, lightShaderInfo.enabledLoc, &enabled, SHADER_UNIFORM_INT);
                SetShaderValue(shader, lightShaderInfo.typeLoc, &enabled, SHADER_UNIFORM_INT);

                // Send to shader light position values
                const float position_data[3] = {position.x, position.y, position.z};
                SetShaderValue(shader, lightShaderInfo.positionLoc, position_data, SHADER_UNIFORM_VEC3);

                // Send to shader light target position values
                const float target_data[3] = {target.x, target.y, target.z};
                SetShaderValue(shader, lightShaderInfo.targetLoc, target_data, SHADER_UNIFORM_VEC3);

                // Send to shader light color values
                const float color_data[4] = {
                    static_cast<float>(color.r) / 255.f,
                    static_cast<float>(color.g) / 255.f,
                    static_cast<float>(color.b) / 255.f,
                    static_cast<float>(color.a) / (float)255.f
                };
                SetShaderValue(shader, lightShaderInfo.colorLoc, color_data, SHADER_UNIFORM_VEC4);
            }
        }

        void Renderer::startRender()
        {
            BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
        }

        void Renderer::endRender()
        {
            EndMode3D();
            EndDrawing();
        }

        void Renderer::renderMesh(const Mesh &mesh, const Material &material, const Vector3& position, const Quaternion& rotation, const float scale)
        {
            const auto matrix = makeTransformationMatrix(position, rotation, scale);
            DrawMesh(mesh, material, matrix);
        }
    }
} // SS3D
