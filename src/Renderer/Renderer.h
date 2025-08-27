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

namespace SS3D
{
    namespace Renderer
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
        };

        class Renderer
        {
        public:
            Renderer(int width, int height);
            void initialize(const std::filesystem::path& shadersPath);
            void updateLight(LightHandle id, const Vector3& position, const Vector3& target, const Color& color,
                             bool enabled = true);
            void startRender();
            void endRender();
            void renderMesh(const Mesh& mesh, const Material& material, const Vector3& position,
                            const Quaternion& rotation,
                            float scale) const;

            const Shader& getShader(const std::string& shader_name) const { return shaders.at(shader_name); }

            ::Camera camera{};

        private:
            std::set<Model> models;
            std::unordered_map<std::string, std::array<LightShaderInformation, MAX_LIGHTS>> lightsShaderInformation;
            std::unordered_map<std::string, Shader> shaders;

            int width, height;

            bool inRender{false};

            void setupLightShaderInformation();
            static Matrix makeTransformationMatrix(const Vector3& position, const Quaternion& rotation, float scale);
        };
    }
} // SS3D

#endif //INC_3DSOLARSYSTEM_RENDERE_H