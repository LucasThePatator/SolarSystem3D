
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
                           {0.f, 5, 0},
                           {0, 0, 0, 1});

    solarSystem.createBody("Mars",
                           6.39e23,
                           3389.5,
                           {5.f, 20, 0},
                           {0, 0, 0, 1});

    solarSystem.createBody("Sun",
                           1.988e30,
                           6960,
                           {0.f, 0, 0},
                           {0, 0, 0, 1}, std::nullopt, "sun");

    while (!WindowShouldClose())
    {
        solarSystem.update(30);
        solarSystem.render();
    }

    return 0;
}
