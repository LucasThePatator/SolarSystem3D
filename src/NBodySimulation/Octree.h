//
// Created by lucas on 8/25/25.
//

#ifndef INC_3DSOLARSYSTEM_OCTREE_H
#define INC_3DSOLARSYSTEM_OCTREE_H

#include <array>
#include <memory>
#include <raylib.h>
#include <vector>

namespace SS3D
{
    class Octree
    {
    public:
        struct Node
        {
            std::array<std::shared_ptr<Node>, 8> children;
            std::shared_ptr<Node> parent;

            bool external = true;
            bool hasBody = false;
            size_t bodyIndex{0};
            Vector3 position{0, 0, 0};
            float mass{0.f};
            Vector3 cornerMin{0, 0, 0};
            Vector3 cornerMax{0, 0, 0};
            float size{0.f};

            void updateSize();

            [[nodiscard]] bool inNode(const Vector3& pos) const;

            void setBody(const Vector3& pos, float newMass, size_t newBodyIndex);
            void setInternal();
            void update(const Vector3& pos, float newMass);
        };

        std::shared_ptr<Node> root;
        explicit Octree(const std::vector<Vector3>& positions, const std::vector<float>& masses);
        [[nodiscard]] std::string toDot() const;

    private:
        void computeWorldBounds(const std::vector<Vector3>& positions);
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_OCTREE_H