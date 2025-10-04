//
// Created by lucas on 7/30/25.
//

#ifndef INC_3DSOLARSYSTEM_RENDERE_H
#define INC_3DSOLARSYSTEM_RENDERE_H

#include <raylib.h>

#include <array>
#include <unordered_map>
#include <set>
#include <string>
#include <filesystem>
#include <spdlog/spdlog.h>

#include "PostProcessing.h"
#include <variant>

namespace SS3D::Renderer
{
    constexpr size_t MAX_LIGHTS = 4;

    using LightHandle = int32_t;

    struct LightShaderInformation
    {
        int enabledLoc{-1};
        int typeLoc{-1};
        int positionLoc{-1};
        int targetLoc{-1};
        int colorLoc{-1};
        int attenuationLoc{-1};
        int powerLoc{-1};
    };

    using ShaderLocations = std::unordered_map<std::string, int>;

    class Renderer
    {
    public:
        Renderer(int width, int height);
        void initialize(const std::filesystem::path& shadersPath);
        void updateLight(LightHandle id, const Vector3& position, const Vector3& target, const Color& color,
                         float power, bool enabled = true);
        void setTime(float dt);
        void startRender();
        void endRender();
        void renderMesh(const Mesh& mesh, const Material& material, const Vector3& position,
                        const Quaternion& rotation,
                        float scale,
                        const std::unordered_map<std::string, std::variant<int, float, Vector3>>& renderParameters =
                            {});

        const Shader& getShader(const std::string& shader_name) const { return shaders.at(shader_name); }
        void setupSkybox(const std::filesystem::path& skyboxImagePath);
        void renderSkybox() const;

        template <typename T>
        void addPostProcessing(const std::string& name)
        {
            postProcessings.push_back(std::make_shared<T>(width, height, &renderTarget));
        }

        void renderModel(const Model& model, const Vector3& position, const Quaternion& attitude, float scale,
                         const std::unordered_map<std::string, std::variant<int, float, Vector3>>& renderParameters =
                             {});


        ::Camera camera{};

    private:
        std::set<Model> models;
        std::unordered_map<std::string, std::array<LightShaderInformation, MAX_LIGHTS>> lightsShaderInformation;
        std::unordered_map<std::string, Shader> shaders;
        std::unordered_map<unsigned int, ShaderLocations> shaderLocations;

        RenderTexture2D renderTarget;

        Mesh skyboxCube;
        Model skybox;

        float currentRenderTime{0};
        int width, height;
        bool inRender{false};

        std::vector<std::shared_ptr<PostProcessing>> postProcessings;

        void setupLightShaderInformation();
        static Matrix makeTransformationMatrix(const Vector3& position, const Quaternion& rotation, float scale);

        int getUniformLocation(const Shader& shader, const std::string& name);
        void setShaderUniformValue(const Shader& shader, const std::string& name, int value);
        void setShaderUniformValue(const Shader& shader, const std::string& name, float value);
        void setShaderUniformValue(const Shader& shader, const std::string& name, Vector3 value);
    };
}

// SS3D

#endif //INC_3DSOLARSYSTEM_RENDERE_H
