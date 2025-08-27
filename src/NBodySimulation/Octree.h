//
// Created by lucas on 8/25/25.
//

#ifndef INC_3DSOLARSYSTEM_OCTREE_H
#define INC_3DSOLARSYSTEM_OCTREE_H

#include <array>
#include <memory>
#include <raylib.h>
#include <vector>

namespace SS3D {
    class Octree {
        struct Node {
            std::array<std::shared_ptr<Node>, 8> children;
            std::shared_ptr<Node> parent;

            bool external = false;
            Vector3 cornerMin, cornerMax;
            Vector3 position{0, 0, 0};
            double mass{0.};
        };

    public:

        explicit Octree(const std::vector<Vector3> &positions);
    };
} // SS3D

#endif //INC_3DSOLARSYSTEM_OCTREE_H
