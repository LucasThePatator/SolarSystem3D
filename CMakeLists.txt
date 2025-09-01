cmake_minimum_required(VERSION 3.28)
project(3DSolarSystem)

add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 23)
include_directories(/usr/local/include ./)

add_executable(3DSolarSystem src/main.cpp
        src/EntityComponentSystem/Types.h
        src/EntityComponentSystem/Systems/System.cpp
        src/EntityComponentSystem/Systems/System.h
        src/EntityComponentSystem/Systems/PhysicsSystem.cpp
        src/EntityComponentSystem/Systems/PhysicsSystem.h
        src/EntityComponentSystem/Systems/RenderingSystem.cpp
        src/EntityComponentSystem/Systems/RenderingSystem.h
        src/EntityComponentSystem/Components/Graphics.cpp
        src/EntityComponentSystem/Components/Graphics.h
        src/EntityComponentSystem/Components/Transform.cpp
        src/EntityComponentSystem/Components/Transform.h
        src/EntityComponentSystem/ComponentCollection.h
        src/EntityComponentSystem/Components/Components.h
        src/EntityComponentSystem/EntityManager.cpp
        src/EntityComponentSystem/EntityManager.h
        src/EntityComponentSystem/Components/Motion.cpp
        src/EntityComponentSystem/Components/Motion.h
        src/EntityComponentSystem/Components/Orbiting.h
        src/EntityComponentSystem/Systems/MovementSystem.cpp
        src/EntityComponentSystem/Systems/MovementSystem.h
        src/EntityComponentSystem/Systems/Systems.h
        src/EntityComponentSystem/SystemRegister.cpp
        src/EntityComponentSystem/SystemRegister.h
        src/EntityComponentSystem/ComponentsRegister.h
        src/EntityComponentSystem/ComponentsRegister.cpp
        src/EntityComponentSystem/EntityComponentSystem.cpp
        src/EntityComponentSystem/EntityComponentSystem.h
        src/SolarSystem.cpp
        src/SolarSystem.h
        src/EntityComponentSystem/Components/Light.cpp
        src/EntityComponentSystem/Components/Light.h
        src/Renderer/Renderer.cpp
        src/Renderer/Renderer.h
        src/EntityComponentSystem/Systems/LightingSystem.cpp
        src/EntityComponentSystem/Systems/LightingSystem.h
        src/Log.h
        src/Log.cpp
        src/EntityComponentSystem/Components/Camera.h
        src/EntityComponentSystem/Systems/ControlsSystem.cpp
        src/EntityComponentSystem/Systems/ControlsSystem.h
        src/NBodySimulation/Octree.cpp
        src/NBodySimulation/Octree.h
        src/NBodySimulation/BarnesHut.cpp
        src/NBodySimulation/BarnesHut.h
        src/Renderer/raygui.h
)

link_directories(/usr/local/lib)
target_link_libraries(3DSolarSystem PUBLIC raylib lua)