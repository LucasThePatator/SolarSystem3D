//
// Created by patator on 21/09/2025.
//

#include "Bloom.h"

#include <spdlog/common.h>

namespace SS3D
{
    Bloom::Bloom(const int width, const int height, RenderTexture2D* inputTexture,
                 const int scale_w, const int scale_h) : PostProcessing(width, height, inputTexture, scale_w, scale_h)
    {
        thresholdShader = LoadShader(nullptr, (shadersDirPath / "brightness_threshold.fs").c_str());
        thresholdLocation = GetShaderLocation(thresholdShader, "threshold");

        gaussianBlurShader = LoadShader(nullptr, (shadersDirPath / "gaussian_blur.fs").c_str());
        horizontalLocation = GetShaderLocation(gaussianBlurShader, "horizontal");

        bloomShader = LoadShader(nullptr, (shadersDirPath / "bloom.fs").c_str());
        bloomTextureLocation = GetShaderLocation(bloomShader, "texture1");
        exposureLocation = GetShaderLocation(bloomShader, "exposure");

        temporaryTexture0 = LoadRenderTextureWithFormat(width, height, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32);
        temporaryTexture1 = LoadRenderTextureWithFormat(width, height, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32);

        SetShaderValue(thresholdShader, thresholdLocation, (void*)&threshold, SHADER_UNIFORM_FLOAT);
    }

    void Bloom::run()
    {
        BeginTextureMode(temporaryTexture0);
        BeginShaderMode(thresholdShader);
        DrawTextureRec(input->texture, (Rectangle){
                           0, 0, static_cast<float>(input->texture.width), static_cast<float>(-input->texture.height)
                       },
                       (Vector2){0, 0}, WHITE);
        EndShaderMode();
        EndTextureMode();

        for (int blurI = 0; blurI < repeats; blurI++)
        {
            horizontal = 0;
            SetShaderValue(gaussianBlurShader, horizontalLocation, (void*)&horizontal, SHADER_UNIFORM_INT);
            BeginTextureMode(temporaryTexture1);
            ClearBackground(BLACK);
            BeginShaderMode(gaussianBlurShader);
            DrawTextureRec(temporaryTexture0.texture, (Rectangle){
                               0, 0, static_cast<float>(input->texture.width),
                               static_cast<float>(-input->texture.height)
                           },
                           (Vector2){0, 0}, WHITE);
            EndShaderMode();
            EndTextureMode();

            horizontal = 1;
            SetShaderValue(gaussianBlurShader, horizontalLocation, (void*)&horizontal, SHADER_UNIFORM_INT);
            BeginTextureMode(temporaryTexture0);
            ClearBackground(BLACK);
            BeginShaderMode(gaussianBlurShader);
            DrawTextureRec(temporaryTexture1.texture, (Rectangle){
                               0, 0, static_cast<float>(input->texture.width),
                               static_cast<float>(-input->texture.height)
                           },
                           (Vector2){0, 0}, WHITE);
            EndShaderMode();
            EndTextureMode();
        }

        BeginTextureMode(output);
        ClearBackground(BLACK);
        BeginShaderMode(bloomShader);
        SetShaderValueTexture(bloomShader, bloomTextureLocation, temporaryTexture0.texture);
        SetShaderValue(bloomShader, exposureLocation, &exposure, SHADER_UNIFORM_FLOAT);
        DrawTextureRec(input->texture, (Rectangle){
                           0, 0, static_cast<float>(input->texture.width), static_cast<float>(-input->texture.height)
                       },
                       (Vector2){0, 0}, WHITE);
        EndShaderMode();
        EndTextureMode();
    }
} // SS3D
