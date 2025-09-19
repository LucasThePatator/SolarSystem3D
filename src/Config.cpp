//
// Created by patator on 13/09/2025.
//

#include "Config.h"

#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include <spdlog/spdlog.h>

namespace SS3D
{
    std::optional<double> getNumberField(lua_State* L, const char* key)
    {
        if (lua_getfield(L, -1, key) == LUA_TNUMBER)
        {
            const double result = lua_tonumber(L, -1);
            lua_pop(L, 1); /* remove number */
            return result;
        }
        lua_pop(L, 1);
        return {};
    }

    std::optional<std::string> getStringField(lua_State* L, const char* key)
    {
        if (lua_getfield(L, -1, key) == LUA_TSTRING)
        {
            std::string result{lua_tostring(L, -1)};
            lua_pop(L, 1); /* remove number */
            return result;
        }
        lua_pop(L, 1);
        return {};
    }

    template <std::size_t size>
    std::array<float, size> getArrayField(lua_State* L, const char* key)
    {
        std::array<float, size> result{};
        if (!lua_getfield(L, -1, key))
            exit(2);

        if (!lua_istable(L, -1))
            exit(3);

        for (int i = 0; i < size; i++)
        {
            if (!lua_geti(L, -1, i + 1))
                exit(2);

            if (lua_isnumber(L, -1))
                result[i] = lua_tonumber(L, -1);
            else if (lua_isinteger(L, -1))
                result[i] = lua_tointeger(L, -1);
            else
                exit(5);

            result[i] = lua_tonumber(L, -1);
            lua_pop(L, 1); //remove index from the stack
        }
        lua_pop(L, 1); /* remove number */
        return result;
    }

    Vector3 Array2Vector3(const std::array<float, 3>& value)
    {
        return Vector3(value[0], value[1], value[2]);
    }

    Vector4 Array2Vector4(const std::array<float, 4>& value)
    {
        return Vector4(value[0], value[1], value[2], value[3]);
    }

    BodySpawnConfig getBodySpawnConfig(lua_State* L)
    {
        BodySpawnConfig config{};
        config.name = *getStringField(L, "name");
        config.mass = static_cast<float>(*getNumberField(L, "mass"));
        config.radius = static_cast<float>(*getNumberField(L, "radius"));
        config.scale = static_cast<float>(*getNumberField(L, "scale"));
        config.position = Array2Vector3(getArrayField<3>(L, "position"));
        config.speed = Array2Vector3(getArrayField<3>(L, "velocity"));
        config.rotationSpeed = Array2Vector3(getArrayField<3>(L, "attitude"));
        config.refBodyName = *getStringField(L, "refBodyName");
        config.shaderName = *getStringField(L, "shaderName");
        if (const auto lightIntensity = getNumberField(L, "lightIntensity"); lightIntensity.has_value())
        {
            config.lightIntensity = static_cast<float>(*lightIntensity);
            config.isLight = true;
        }

        return config;
    }

    Config::Config(const std::filesystem::path& path)
    {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        luaopen_math(L);

        if (luaL_dofile(L, path.c_str()) == LUA_OK)
        {
            lua_pop(L, 1);
        }
        else
        {
            spdlog::error("Failed to load lua script.");
        }

        lua_getglobal(L, "Scale");
        modelScale = lua_tonumber(L, -1);
        lua_remove(L, 1);
        lua_getglobal(L, "Width");
        width = lua_tonumber(L, -1);
        lua_remove(L, 1);
        lua_getglobal(L, "Height");
        height = lua_tonumber(L, -1);
        lua_remove(L, 1);
        lua_getglobal(L, "ResourcePath");
        resourcePath = lua_tostring(L, -1);
        lua_remove(L, 1);
        lua_getglobal(L, "ShaderPath");
        shaderPath = lua_tostring(L, -1);
        lua_remove(L, 1);
        lua_getglobal(L, "SkyboxTexturePath");
        skyboxTexturePath = lua_tostring(L, -1);
        //lua_remove(L, 1);

        lua_getglobal(L, "Bodies");

        for (int i = 0; true; i++)
        {
            if (lua_geti(L, -1, i + 1) == LUA_TNIL)
                break;
            bodySpawnConfigs.push_back(getBodySpawnConfig(L));
            lua_pop(L, 1);
        }

        lua_pop(L, 2);

        lua_close(L);
    }
} // SS3D
