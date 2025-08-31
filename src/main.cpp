
#include "Log.h"

#include "SolarSystem.h"
#include "EntityComponentSystem/EntityComponentSystem.h"

using namespace SS3D;

int main(int argc, char* argv[])
{
    initLogging();
    spdlog::info("Started 3DSolarSystem");

    EntityComponentSystem ecs;
    SolarSystem solarSystem(ecs, std::filesystem::path(argv[1]));

    double lastPrintTime = 0.;
    while (!WindowShouldClose())
    {
        solarSystem.update(0.0166);
        solarSystem.render();

        if (const auto elapsedTime = GetTime(); elapsedTime - lastPrintTime >= 3.f)
        {
            auto FPS = GetFPS();
            spdlog::debug("FPS: {0}", FPS);
            lastPrintTime = elapsedTime;
        }
    }

    return 0;
}
