//
// Created by lucas on 8/25/25.
//

#include "Octree.h"

#include <algorithm>
#include <limits>

namespace SS3D
{
    Octree::Octree(const std::vector<Vector3>& positions)
    {
        //We need the boundaries of the world
        Vector3 cornerMin{
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()
        };
        Vector3 cornerMax{
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::lowest()
        };

        for (const auto& [x, y, z] : positions)
        {
            cornerMin.x = std::min(x, cornerMin.x);
            cornerMin.y = std::min(y, cornerMin.y);
            cornerMin.z = std::min(z, cornerMin.z);

            cornerMax.x = std::max(x, cornerMax.x);
            cornerMax.y = std::max(y, cornerMax.y);
            cornerMax.z = std::max(z, cornerMax.z);
        }

        bool newSplit = true;
        while (newSplit)
        {
            newSplit = false;
        }
    }
} // SS3D
