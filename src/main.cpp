#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cstdio>

#include "SolarSystem.h"
#include "EntityComponentSystem/EntityComponentSystem.h"
using namespace SS3D;

// ReSharper disable once CppParameterMayBeConst
void raylibLogCallback(const int logLevel, const char *text, va_list args)
{
    char textBuffer[1024];
    vsprintf(textBuffer, text, args);
    switch (logLevel)
    {
        case LOG_INFO:
        spdlog::info(textBuffer);
        break;
        case LOG_WARNING:
        spdlog::warn(textBuffer);
        break;
        case LOG_FATAL:
        case LOG_ERROR:
        spdlog::error(textBuffer);
        break;
        case LOG_DEBUG:
        spdlog::debug(textBuffer);
        break;
        case LOG_TRACE:
        spdlog::trace(textBuffer);
        break;
        default:
        spdlog::error(textBuffer);
        break;
    }
}

int main(int argc, char* argv[])
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Started 3DSolarSystem");

    SetTraceLogLevel(LOG_ALL);
    SetTraceLogCallback(raylibLogCallback);

    EntityComponentSystem ecs;
    SolarSystem solarSystem(ecs, std::filesystem::path("/mnt/e/Data/SS3D"));

    solarSystem.createBody("Earth",
                           5.972e24,
                           6378,
                           {0, 0, 0},
                           {0, 0, 0, 1});

    while (!WindowShouldClose())
    {
        solarSystem.update(30);
        solarSystem.render();
    }

    return 0;
}
