//
// Created by patator on 21/09/2025.
//

#ifndef INC_3DSOLARSYSTEM_BLOOM_H
#define INC_3DSOLARSYSTEM_BLOOM_H
#include "PostProcessing.h"

namespace SS3D
{
    class Bloom : public PostProcessing
    {
    public:
        Bloom(int width, int height, RenderTexture2D* inputTexture,
              int scale_w = 1, int scale_h = 1);

        ~Bloom()
        {
        };
        void run() override;

    private:
        Shader thresholdShader{};
        int thresholdLocation;
        float threshold{1.f};

        Shader gaussianBlurShader{};
        int repeats{1};
        int horizontalLocation{0};
        int horizontal{1};

        int bloomTextureLocation{0};
        Shader bloomShader{};

        RenderTexture2D temporaryTexture0{};
        RenderTexture2D temporaryTexture1{};
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_BLOOM_H
