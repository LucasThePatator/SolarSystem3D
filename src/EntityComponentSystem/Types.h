//
// Created by lucas on 6/28/25.
//

#ifndef TYPES_H
#define TYPES_H

#include <bitset>

namespace SS3D
{
    using Entity = uint32_t;
    constexpr uint32_t MAX_ENTITIES{5000u};

    using ComponentTypeID = uint32_t;
    using ComponentInstance = uint32_t;
    using Signature = std::bitset<8 * sizeof(ComponentTypeID)>;
}

#endif //TYPES_H