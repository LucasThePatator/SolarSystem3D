//
// Created by patator on 30/08/2025.
//

#include <iostream>
#include "../src/NBodySimulation/Octree.h"
#include <raylib.h>
#include <random>

constexpr int nb_points = 5;
std::random_device rd; // Will be used to obtain a seed for the random number engine
//std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
std::mt19937 gen(0); // Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<float> dis_positions(-2.0, 2.0);
std::uniform_real_distribution<float> dis_mass(0.0, 1.0);

Vector3 randomVector()
{
    return Vector3{dis_positions(gen), dis_positions(gen), dis_positions(gen)};
}

int main()
{
    std::cout << "Testing octree..." << std::endl;

    std::vector<Vector3> points;
    std::vector<float> masses;

    for (int i = 0; i < nb_points; i++)
    {
        points.push_back(randomVector());
        masses.push_back(dis_mass(gen));
    }
    const SS3D::Octree octree(points, masses);
    std::cout << octree.toDot();
}
