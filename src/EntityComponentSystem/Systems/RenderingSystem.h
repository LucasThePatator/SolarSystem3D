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
    // Light data should really be a component. But that requires an architecture of component types that doesn't exist yet
    // Light type
    enum class LightType : int
    {
        DIRECTIONAL = 0,
        POINT
    } ;

    // Light data
    struct Light
    {
        LightType type;
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
    };


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
        Light sunLight;

        const std::filesystem::path shadersPath;
        std::unordered_map<std::string, Shader> shaders;

        static Matrix makeTransformationMatrix(const Vector3& position, const Quaternion& rotation, float scale);
        static Light CreateLight(LightType type, const Vector3 &position, const Vector3 &target, const Color &color, const Shader &shader, const uint8_t lightIndex);
    };
}


#endif //RENDERINGSYSTEM_H
