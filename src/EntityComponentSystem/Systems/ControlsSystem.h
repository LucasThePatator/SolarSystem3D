//
// Created by lucas on 8/20/25.
//

#ifndef INC_3DSOLARSYSTEM_CONTROLSSYSTEM_H
#define INC_3DSOLARSYSTEM_CONTROLSSYSTEM_H

#include "System.h"

#include <raylib.h>
#include <unordered_map>

namespace SS3D
{
    namespace Renderer
    {
        class Renderer;
    }

    class ControlsSystem : public System
    {
    public:
        explicit ControlsSystem(const std::shared_ptr<SS3D::Renderer::Renderer>& renderer);

        void initialize() override;
        void update(float dt) override;
        void render() override;

        std::shared_ptr<SS3D::Renderer::Renderer> renderer;
        std::unordered_map<std::string, void*> parameterPointers;

    private:
        void renderGui();

        //Gui status
        int bodyIndex{0};
        bool checked{false};

        Vector3 currentTarget{};
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_CONTROLSSYSTEM_H