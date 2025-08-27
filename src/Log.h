//
// Created by lucas on 7/31/25.
//

#ifndef INC_3DSOLARSYSTEM_LOG_H
#define INC_3DSOLARSYSTEM_LOG_H

#include <spdlog/spdlog.h>

namespace SS3D
{
    // ReSharper disable once CppParameterMayBeConst
    void raylibLogCallback(int logLevel, const char* text, va_list args);
    void initLogging();
}

#endif //INC_3DSOLARSYSTEM_LOG_H
