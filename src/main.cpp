#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "SolarSystem.h"
#include "EntityComponentSystem/EntityComponentSystem.h"
using namespace SS3D;

int main(int argc, char* argv[])
{
    spdlog::info("Started 3DSolarSystem");
    spdlog::set_level(spdlog::level::debug);

    EntityComponentSystem ecs;
    SolarSystem solarSystem(ecs, std::filesystem::path("/mnt/e/Data/SS3D"));

    solarSystem.createBody("Earth",
                           5.972e24,
                           6378,
                           {0, 0, 0},
                           {0, 0, 0, 1});

    bool running = true;
    while (running)
    {
        solarSystem.update(30);
        solarSystem.render();
    }

    return 0;
}
