//
// Created by lucas on 7/31/25.
//

#ifndef INC_3DSOLARSYSTEM_LIGHTINGSYSTEM_H
#define INC_3DSOLARSYSTEM_LIGHTINGSYSTEM_H
#include "System.h"

namespace SS3D
{
    namespace Renderer
    {
        class Renderer;
    }

    class LightingSystem : public System
    {
    public:
        explicit LightingSystem(const std::shared_ptr<SS3D::Renderer::Renderer>& renderer);

        void initialize() override;
        void update(float dt) override;
        void render() override;

    private:
        std::shared_ptr<SS3D::Renderer::Renderer> renderer;
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_LIGHTINGSYSTEM_H