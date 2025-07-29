//
// Created by lucas on 6/28/25.
//

#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include <filesystem>
#include <unordered_map>
#include "System.h"
#include "../Components/Graphics.h"

namespace SS3D
{
    class RenderingSystem : public System
    {
    public :
        explicit RenderingSystem(const std::filesystem::path &shadersPath);

        void initialize() override;
        void update(float dt) override;
        void render() override;

    private:
        int width, height;
        Camera camera;

        const std::filesystem::path shadersPath;
        std::unordered_map<std::string, Shader> shaders;

        static Matrix makeTransformationMatrix(const Vector3& position, const Quaternion& rotation, float scale);
    };
}


#endif //RENDERINGSYSTEM_H
