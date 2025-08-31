enable_testing()

set(CMAKE_CXX_STANDARD 23)

add_executable(
        test_octree
        test_octree.cpp
        ../src/NBodySimulation/Octree.cpp
)
target_link_libraries(test_octree raylib)
add_test(NAME test_octree COMMAND test_octree)

add_executable(
        test_barneshut
        test_barneshut.cpp
        ../src/NBodySimulation/Octree.cpp
        ../src/NBodySimulation/BarnesHut.cpp
)

target_link_libraries(test_barneshut raylib)
add_test(NAME test_barneshut COMMAND test_barneshut)