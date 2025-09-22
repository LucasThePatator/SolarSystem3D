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
        horizontal = GetShaderLocation(gaussianBlurShader, "horizontal");

        bloomShader = LoadShader(nullptr, (shadersDirPath / "bloom.fs").c_str());
        bloomTextureLocation = GetShaderLocation(bloomShader, "bloomTexture");

        temporaryTexture0 = LoadRenderTexture(width, height);
        temporaryTexture1 = LoadRenderTexture(width, height);


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

        horizontal = false;
        SetShaderValue(gaussianBlurShader, horizontalLocation, (void*)&horizontal, SHADER_UNIFORM_UINT);
        BeginTextureMode(temporaryTexture1);
        ClearBackground(BLACK);
        BeginShaderMode(gaussianBlurShader);
        DrawTextureRec(temporaryTexture0.texture, (Rectangle){
                           0, 0, static_cast<float>(input->texture.width), static_cast<float>(-input->texture.height)
                       },
                       (Vector2){0, 0}, WHITE);
        EndShaderMode();
        EndTextureMode();

        horizontal = true;
        SetShaderValue(gaussianBlurShader, horizontalLocation, (void*)&horizontal, SHADER_UNIFORM_UINT);
        BeginTextureMode(temporaryTexture0);
        ClearBackground(BLACK);
        BeginShaderMode(gaussianBlurShader);
        DrawTextureRec(temporaryTexture1.texture, (Rectangle){
                           0, 0, static_cast<float>(input->texture.width), static_cast<float>(-input->texture.height)
                       },
                       (Vector2){0, 0}, WHITE);
        EndShaderMode();
        EndTextureMode();

        SetShaderValueTexture(bloomShader, bloomTextureLocation, input->texture);
        BeginTextureMode(output);
        ClearBackground(BLACK);
        BeginBlendMode(BLEND_ADDITIVE);
        DrawTextureRec(input->texture, (Rectangle){
                           0, 0, static_cast<float>(input->texture.width), static_cast<float>(-input->texture.height)
                       },
                       (Vector2){0, 0}, WHITE);
        DrawTextureRec(temporaryTexture0.texture, (Rectangle){
                           0, 0, static_cast<float>(input->texture.width), static_cast<float>(-input->texture.height)
                       },
                       (Vector2){0, 0}, WHITE);
        EndBlendMode();
        EndTextureMode();
    }
} // SS3D
