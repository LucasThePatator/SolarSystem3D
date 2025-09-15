//
// Created by lucas on 7/30/25.
//

#include "Renderer.h"

#include <rlgl.h>
#include <raymath.h>
#include <spdlog/spdlog.h>
#include "raygui.h"
#include "src/EntityComponentSystem/EntityManager.h"

#include "src/EntityComponentSystem/Components/Light.h"
#define GLSL_VERSION 420


namespace SS3D::Renderer
{
    Renderer::Renderer(const int width, const int height) :
        width(width), height(height)
    {
    }

    void Renderer::initialize(const std::filesystem::path& shadersPath)
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
        InitWindow(width, height, "SolarSystem 3D");

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

                shader.locs[SHADER_LOC_MAP_DIFFUSE] = GetShaderLocation(shader, "diffuseMap");  // Choose any name you use on the shader for texture used as diffuse
                shader.locs[SHADER_LOC_MAP_SPECULAR] = GetShaderLocation(shader, "specularMap"); // Choose any name you use on the shader for texture used as specular/metalness
                shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(shader, "normalMap");   // Choose any name you use on the shader for texture used as normal
                shader.locs[SHADER_LOC_MAP_EMISSION] = GetShaderLocation(shader, "nightEmissionMap");   // Choose any name you use on the shader for texture used as normal
                shader.locs[SHADER_LOC_MAP_CUBEMAP] = GetShaderLocation(shader, "cubeMap");
                // Choose any name you use on the shader for texture used as diffuse
            }
        }

        setupLightShaderInformation();
    }

    void Renderer::setupSkybox(const std::filesystem::path& skyboxImagePath)
    {
        skyboxCube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skybox = LoadModelFromMesh(skyboxCube);

        skybox.materials[0].shader = shaders.at("skybox");

        const Image skyboxImg = LoadImage(skyboxImagePath.c_str());
        skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(
            skyboxImg, CUBEMAP_LAYOUT_AUTO_DETECT);
        UnloadImage(skyboxImg);
    }

    void Renderer::renderSkybox()
    {
        // We are inside the cube, we need to disable backface culling!
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
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

        const Matrix RT = MatrixMultiply(R, T);
        const Matrix SRT = MatrixMultiply(S, RT);
        return SRT;
    }

    void Renderer::updateLight(const LightHandle id, const Vector3& position, const Vector3& target,
                               const Color& color, const bool enabled)
    {
        for (const auto& [shaderName, shader] : shaders)
        {
            const auto& lightShaderInfo = lightsShaderInformation[shaderName][id];
            // Send to shader light enabled state and type
            const int enabledInt = enabled ? 1 : 0;
            constexpr auto type = LightType::POINT;
            SetShaderValue(shader, lightShaderInfo.enabledLoc, &enabledInt, SHADER_UNIFORM_INT);
            SetShaderValue(shader, lightShaderInfo.typeLoc, &type, SHADER_UNIFORM_INT);

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
                static_cast<float>(color.a) / 255.f
            };
            SetShaderValue(shader, lightShaderInfo.colorLoc, color_data, SHADER_UNIFORM_VEC4);
        }
    }

    void Renderer::startRender()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        inRender = true;
    }

    void Renderer::endRender()
    {
        EndDrawing();
        inRender = false;
    }

    void Renderer::renderMesh(const Mesh& mesh, const Material& material, const Vector3& position,
                              const Quaternion& rotation, const float scale) const
    {
        if (!inRender)
        {
            throw std::runtime_error("Renderer::renderMesh: Not in render mode");
        }

        if (!IsMaterialValid(material))
        {
            throw std::runtime_error("Material is not valid!");
        }

        const auto matrix = makeTransformationMatrix(position, rotation, scale);
        DrawMesh(mesh, material, matrix);

    }
}

// SS3D
