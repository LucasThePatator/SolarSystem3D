//
// Created by lucas on 6/28/25.
//

#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include "System.h"
#include "../Components/Graphics.h"
#include "src/Renderer/Renderer.h"

namespace SS3D
{
    namespace Renderer
    {
        class Renderer;
    }

    class RenderingSystem : public System
    {
    public :
        explicit RenderingSystem(const std::shared_ptr<SS3D::Renderer::Renderer> &renderer);

        void initialize() override;
        void update(float dt) override;
        void render() override;

    private:
        std::shared_ptr<SS3D::Renderer::Renderer> renderer;
    };
}


#endif //RENDERINGSYSTEM_H
