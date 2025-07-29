//
// Created by lucas on 6/28/25.
//

#include <raylib.h>
#include "RenderingSystem.h"

#include "../EntityManager.h"
#include "../ComponentsRegister.h"

namespace SS3D
{
    // Light data
    struct
    {
        int type;
        bool enabled;
        Vector3 position;
        Vector3 target;
        Color color;
        float attenuation;

        // Shader locations
        int enabledLoc;
        int typeLoc;
        int positionLoc;
        int targetLoc;
        int colorLoc;
        int attenuationLoc;
    } Light;

    // Light type
    enum
    {
        LIGHT_DIRECTIONAL = 0,
        LIGHT_POINT
    } LightType;

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
}
