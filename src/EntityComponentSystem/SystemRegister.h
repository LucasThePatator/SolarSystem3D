//
// Created by lucas on 7/24/25.
//

#ifndef INC_3DSOLARSYSTEM_SYSTEMMANAGER_H
#define INC_3DSOLARSYSTEM_SYSTEMMANAGER_H
#include <memory>
#include <unordered_map>
#include <spdlog/spdlog.h>

#include "Types.h"

namespace SS3D
{
    class System;
    class ComponentsRegister;

    class SystemRegister
    {
    public:
        SystemRegister() = default;

        template <typename SystemType, typename... Args>
        std::shared_ptr<SystemType> registerSystem(const Signature signature, Args... args)
        {
            const auto systemName = typeid(SystemType).name();
            if (systems.contains(systemName))
            {
                spdlog::get("error")->debug("System {} already registered", systemName);
                throw std::runtime_error("System already registered");
            }

            auto system = std::make_shared<SystemType>(args...);
            systems[systemName] = system;
            signatures[systemName] = signature;

            system->setComponentsRegister(componentsRegister);

            return system;
        }

        template <typename SystemType>
        std::shared_ptr<SystemType> getSystem()
        {
            const auto systemName = typeid(SystemType).name();
            return std::static_pointer_cast<SystemType>(systems.at(systemName));
        }

        void setComponentsRegister(const std::shared_ptr<ComponentsRegister>& reg)
        {
            componentsRegister = reg;
        }

        void onEntitySignatureChanged(Entity entity, Signature entitySignature);
        void onEntityDestroyed(Entity entity);

    protected:
        std::unordered_map<const char*, std::shared_ptr<System>> systems{};
        std::unordered_map<const char*, Signature> signatures{};

        std::shared_ptr<ComponentsRegister> componentsRegister{};
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_SYSTEMMANAGER_H
