//
// Created by lucas on 6/28/25.
//

#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include "System.h"

namespace SS3D
{
    namespace Renderer
    {
        class Renderer;
    }

    class RenderingSystem : public System
    {
    public :
        explicit RenderingSystem(const std::shared_ptr<SS3D::Renderer::Renderer>& renderer);

        void initialize() override;
        void update(float dt) override;
        void render() override;

        std::shared_ptr<SS3D::Renderer::Renderer> renderer;

    private:
    };
}


#endif //RENDERINGSYSTEM_H