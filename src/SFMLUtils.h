#pragma once

#include <vector>

namespace utils::sfml
{

    struct Vector2u
    {
    unsigned x;
    unsigned y;
};

template<class DestT>
inline DestT VectorCast(const auto& vec)
{
    return DestT{vec.x, vec.y};
}

template<class V>
bool AreAdjacent(const V& first, const V& second);

template<class V>
std::vector<V> GetIntersected(const V& first, const V& second);

} // namespace utils::sfml
