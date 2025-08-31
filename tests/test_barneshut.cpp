//
// Created by patator on 30/08/2025.
//

#include <iostream>
#include "../src/NBodySimulation/BarnesHut.h"
#include <raylib.h>
#include <random>
#include <raymath.h>

constexpr int nb_points = 5;
std::random_device rd; // Will be used to obtain a seed for the random number engine
//std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
std::mt19937 gen(0); // Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<float> dis_positions(-1.f, 1.f);
std::uniform_real_distribution<float> dis_mass(0.f, 1.f);

Vector3 randomVector(const float scale = 1.f)
{
    return Vector3{scale * dis_positions(gen), scale * dis_positions(gen), scale * dis_positions(gen)};
}

int main()
{
    std::cout << "Testing octree..." << std::endl;

    std::vector<Vector3> points;
    std::vector<Vector3> velocities;

    std::vector<float> masses;

    for (int i = 0; i < nb_points; i++)
    {
        points.push_back(randomVector(2));
        masses.push_back(dis_mass(gen));
        velocities.push_back(randomVector(1.f));
    }

    std::vector<Vector3> acceleration;
    constexpr SS3D::BarnesHut bh{.G = 0.01f};
    bh.computeAcceleration(points, masses, acceleration);

    for (const auto& a : acceleration)
    {
        std::cout << a.x << ", " << a.y << ", " << a.z << std::endl;
    }
}
