
#include "Log.h"

#include "SolarSystem.h"
#include "EntityComponentSystem/EntityComponentSystem.h"

using namespace SS3D;

int main(int argc, char* argv[])
{
    initLogging();
    spdlog::info("Started 3DSolarSystem");

    EntityComponentSystem ecs;
    SolarSystem solarSystem(ecs, std::filesystem::path("/mnt/e/Data/SS3D"));

    solarSystem.createBody("Earth",
                           5.972e24,
                           6378,
                           {0.f, 10, 0},
                           {0, 0, 0, 1});

    while (!WindowShouldClose())
    {
        solarSystem.update(30);
        solarSystem.render();
    }

    return 0;
}
