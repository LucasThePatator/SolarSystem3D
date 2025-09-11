
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

    solarSystem.run();

    return 0;
}
