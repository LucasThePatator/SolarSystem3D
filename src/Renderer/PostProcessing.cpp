//
// Created by patator on 21/09/2025.
//

#include "PostProcessing.h"

namespace SS3D
{
    std::filesystem::path PostProcessing::shadersDirPath;

    PostProcessing::PostProcessing(const int width, const int height, RenderTexture2D* inputTexture,
                                   const int scale_w, const int scale_h) :
        input(inputTexture)
    {
        output = LoadRenderTextureWithFormat(width * scale_w, height * scale_h, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32);
    }
}

