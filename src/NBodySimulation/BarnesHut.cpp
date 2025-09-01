//
// Created by patator on 31/08/2025.
//

#include "BarnesHut.h"

#include <deque>
#include <raylib.h>
#include <raymath.h>
#include <toml++/impl/node.hpp>

#include "Octree.h"


namespace SS3D
{
    void BarnesHut::computeAcceleration(const std::vector<Vector3>& positions,
                                        const std::vector<float>& mass, std::vector<Vector3>& acceleration) const
    {
        Octree octree{positions, mass};
        const auto nbPoints = positions.size();
        acceleration.resize(nbPoints, {0.f, 0.f, 0.f});

        //Simple Euler for the time being, maybe we'll be fancier later
        for (auto bodyIndex = 0; bodyIndex < nbPoints; ++bodyIndex)
        {
            const auto& currentBodyPosition = positions[bodyIndex];
            auto& currentBodyAcceleration = acceleration[bodyIndex];

            std::pmr::deque<std::shared_ptr<Octree::Node>> nodeQueue;
            nodeQueue.push_back(octree.root);

            while (!nodeQueue.empty())
            {
                const auto currentNode = nodeQueue.front();
                nodeQueue.pop_front();

                if (bodyIndex == currentNode->bodyIndex && currentNode->external && currentNode->hasBody)
                    continue;

                const auto nodeSize = currentNode->size;
                if (const auto distance = Vector3Distance(currentNode->position, currentBodyPosition);
                    theta * distance > nodeSize || (currentNode->external && currentNode->hasBody))
                //Node is sufficiently far away for approximation or a single body
                {
                    auto currentAcceleration = Vector3Subtract(currentNode->position, currentBodyPosition);
                    currentAcceleration = Vector3Scale(currentAcceleration,
                                                       currentNode->mass / (distance * distance * distance));
                    currentBodyAcceleration = Vector3Add(currentAcceleration, currentBodyAcceleration);
                }
                else if (!currentNode->external)
                {
                    for (const auto& child : currentNode->children)
                        nodeQueue.push_back(child);
                }
            }
            currentBodyAcceleration = Vector3Scale(currentBodyAcceleration, G * scale);
        }
    }
} // SS3D
