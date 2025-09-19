//
// Created by lucas on 8/25/25.
//

#include "Octree.h"

#include <algorithm>
#include <limits>
#include <raymath.h>
#include <sstream>
#include <spdlog/spdlog.h>

namespace SS3D
{
    void Octree::Node::updateSize()
    {
        size = Vector3Distance(cornerMin, cornerMax);
    }

    bool Octree::Node::inNode(const Vector3& pos) const
    {
        if (pos.x < cornerMin.x || pos.x >= cornerMax.x) return false;
        if (pos.y < cornerMin.y || pos.y >= cornerMax.y) return false;
        if (pos.z < cornerMin.z || pos.z >= cornerMax.z) return false;

        return true;
    }

    void Octree::Node::setBody(const Vector3& pos, const float newMass, const size_t newBodyIndex)
    {
        mass = newMass;
        position = pos;
        hasBody = true;
        external = true;
        bodyIndex = newBodyIndex;
    }

    void Octree::Node::setInternal()
    {
        hasBody = false;
        bodyIndex = 0;
        external = false;
    }

    void Octree::Node::update(const Vector3& pos, const float newMass)
    {
        position = Vector3Add(Vector3Scale(position, mass), Vector3Scale(pos, newMass));
        mass += newMass;
        position = Vector3Scale(position, 1.f / mass);
    }

    Octree::Octree(const std::vector<Vector3>& positions, const std::vector<float>& masses)
    {
        if (positions.empty())
        {
            spdlog::error("Can't create octree without position data");
            return;
        }

        root = std::make_shared<Node>();

        computeWorldBounds(positions);

        root->setBody(positions[0], masses[0], 0);
        root->updateSize();

        for (size_t i = 1; i < positions.size(); i++)
        {
            bool inserted = false;
            std::shared_ptr<Node> currentNode = root;
            std::shared_ptr<Node> newNode;
            const auto& currentPosition = positions[i];
            const auto& currentMass = masses[i];

            int depth = 0;
            while (!inserted)
            {
                if (currentNode->external)
                {
                    if (!currentNode->hasBody)
                    {
                        currentNode->setBody(currentPosition, currentMass, i);
                        inserted = true;
                        continue;
                    }

                    //Split the node
                    for (int octantIndex = 0; octantIndex < 8; octantIndex++)
                    {
                        auto& child = currentNode->children[octantIndex];
                        child = std::make_shared<Node>();

                        child->cornerMin = currentNode->cornerMin;
                        child->cornerMax = currentNode->cornerMax;

                        if (octantIndex < 4)
                            child->cornerMax.x = (currentNode->cornerMin.x + currentNode->cornerMax.x) / 2;
                        else
                            child->cornerMin.x = (currentNode->cornerMin.x + currentNode->cornerMax.x) / 2;

                        if ((octantIndex & 2) == 0)
                            child->cornerMax.y = (currentNode->cornerMin.y + currentNode->cornerMax.y) / 2;
                        else
                            child->cornerMin.y = (currentNode->cornerMin.y + currentNode->cornerMax.y) / 2;

                        if (octantIndex % 2 == 0)
                            child->cornerMax.z = (currentNode->cornerMin.z + currentNode->cornerMax.z) / 2;
                        else
                            child->cornerMin.z = (currentNode->cornerMin.z + currentNode->cornerMax.z) / 2;

                        child->updateSize();

                        bool childOfExistingBody = false;
                        if (child->inNode(currentNode->position))
                        {
                            child->setBody(currentNode->position, currentNode->mass, currentNode->bodyIndex);
                            childOfExistingBody = true;
                        }

                        if (child->inNode(currentPosition))
                        {
                            if (childOfExistingBody) //Both positions end up in the same quadrant
                            {
                                newNode = child;
                            }
                            else
                            {
                                child->setBody(currentPosition, currentMass, i);
                                inserted = true;
                            }
                        }
                    }
                    currentNode->setInternal();
                }
                else
                {
                    currentNode->update(currentPosition, currentMass);
                    for (int octantIndex = 0; octantIndex < 8; octantIndex++)
                    {
                        if (currentNode->children[octantIndex]->inNode(currentPosition))
                        {
                            newNode = currentNode->children[octantIndex];
                            break;
                        }
                    }
                }
                currentNode = newNode;
                depth++;
            }
        }
    }

    void writeNode(std::ostringstream& os, size_t& index, const std::shared_ptr<Octree::Node>& node)
    {
        const size_t currentIndex = index;
        if (node->external)
        {
            if (node->hasBody)
                os << '\t' << currentIndex << " [color=red]\n";
            else
                os << '\t' << currentIndex << " [color=green]\n";
            return;
        }
        os << '\t' << currentIndex << " [color=blue]\n";
        for (size_t octantIndex = 0; octantIndex < 8; octantIndex++)
        {
            os << "\t" << currentIndex << " --> " << ++index << "\n";
            writeNode(os, index, node->children[octantIndex]);
        }
    }

    std::string Octree::toDot() const
    {
        std::ostringstream os;
        os << "graph octree {\n";

        size_t currentIndex = 0;
        writeNode(os, currentIndex, root);

        os << "}\n";
        return os.str();
    }

    void Octree::computeWorldBounds(const std::vector<Vector3>& positions)
    {
        root->cornerMin = Vector3{
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()
        };

        root->cornerMax = Vector3{
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::lowest()
        };

        for (const auto& [x, y, z] : positions)
        {
            root->cornerMin.x = std::min(x, root->cornerMin.x);
            root->cornerMin.y = std::min(y, root->cornerMin.y);
            root->cornerMin.z = std::min(z, root->cornerMin.z);

            root->cornerMax.x = std::max(x, root->cornerMax.x);
            root->cornerMax.y = std::max(y, root->cornerMax.y);
            root->cornerMax.z = std::max(z, root->cornerMax.z);
        }

        const auto diagonal = Vector3Subtract(root->cornerMax, root->cornerMin);
        const auto center = Vector3Scale(Vector3Add(root->cornerMin, root->cornerMax), 0.5f);
        const auto maxDimension = std::max({diagonal.x, diagonal.y, diagonal.z});
        const Vector3 cubeDiagonal{maxDimension, maxDimension, maxDimension};

        root->cornerMin = Vector3Subtract(center, Vector3Scale(cubeDiagonal, 0.51f));
        root->cornerMax = Vector3Add(center, Vector3Scale(cubeDiagonal, 0.51f));
    }
} // SS3D
