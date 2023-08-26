#pragma once

#include <vector>

namespace utils::sfml
{

template<class V>
bool AreAdjacent(const V& first, const V& second);

template<class V>
std::vector<V> GetIntersected(const V& first, const V& second);

} // namespace utils::sfml
