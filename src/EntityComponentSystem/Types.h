//
// Created by lucas on 6/28/25.
//

#ifndef TYPES_H
#define TYPES_H

#include <bitset>
#include <cstdint>

namespace SS3D
{
    using Entity = uint32_t;
    constexpr Entity MAX_ENTITIES = 5000;

    using ComponentTypeID = uint8_t;
    using ComponentInstance = uint32_t;

    using Signature = std::bitset<8 * sizeof(ComponentTypeID)>;
}

#endif //TYPES_H
