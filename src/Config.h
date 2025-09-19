//
// Created by patator on 13/09/2025.
//

#ifndef INC_3DSOLARSYSTEM_CONFIG_H
#define INC_3DSOLARSYSTEM_CONFIG_H
#include <filesystem>
#include <raylib.h>
#include <vector>

namespace SS3D
{
    struct BodySpawnConfig
    {
        std::string name;
        float mass;
        float radius;
        float scale;
        Vector3 position;
        Vector3 speed;
        Quaternion attitude;
        Vector3 rotationSpeed;
        std::string refBodyName;
        std::string shaderName;

        bool isLight{false};
        double lightIntensity{0};
    };

    struct Config
    {
        explicit Config(const std::filesystem::path& path);

        double modelScale;
        int width, height;
        std::filesystem::path resourcePath, shaderPath;
        std::vector<BodySpawnConfig> bodySpawnConfigs;
        std::filesystem::path skyboxTexturePath;
    };
} // SS3D


#endif //INC_3DSOLARSYSTEM_CONFIG_H
