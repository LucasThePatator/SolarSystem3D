//
// Created by patator on 21/09/2025.
//

#ifndef INC_3DSOLARSYSTEM_POSTPROCESSING_H
#define INC_3DSOLARSYSTEM_POSTPROCESSING_H

#include <raylib.h>
#include <filesystem>

namespace SS3D
{
    class PostProcessing
    {
    public:
        virtual ~PostProcessing() = default;
        PostProcessing(int width, int height, RenderTexture2D* inputTexture,
                       int scale_w = 1, int scale_h = 1);
        virtual void run() = 0;

        RenderTexture* input;
        RenderTexture output;

        static std::filesystem::path shadersDirPath;
    };
}

#endif //INC_3DSOLARSYSTEM_POSTPROCESSING_H
