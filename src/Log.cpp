//
// Created by lucas on 7/31/25.
//

#include "Log.h"
#include <raylib.h>

namespace SS3D
{
    // ReSharper disable once CppParameterMayBeConst
    void raylibLogCallback(const int logLevel, const char* text, va_list args)
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

    void initLogging()
    {
        spdlog::set_level(spdlog::level::debug);
        SetTraceLogLevel(LOG_ALL);
        SetTraceLogCallback(raylibLogCallback);
    }
}
