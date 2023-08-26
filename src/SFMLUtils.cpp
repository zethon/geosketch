#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "SFMLUtils.h"

namespace utils::sfml
{

template<class V>
bool AreAdjacent(const V& first, V& second)
{
    return (std::abs(first.x - second.x) < 1)
        || (std::abs(first.y - second.y) < 1);
}

template<class V>
std::vector<V> GetIntersected(const V& first, const V& second)
{
    return {};
}


template bool AreAdjacent(const sf::Vector2u&, const sf::Vector2u&);
template bool AreAdjacent(const sf::Vector2i&, const sf::Vector2i&);
template bool AreAdjacent(const sf::Vector2f&, const sf::Vector2f&);

template std::vector<sf::Vector2u> GetIntersected(const sf::Vector2u&, const sf::Vector2u&);
template std::vector<sf::Vector2i> GetIntersected(const sf::Vector2i&, const sf::Vector2i&);
template std::vector<sf::Vector2f> GetIntersected(const sf::Vector2f&, const sf::Vector2f&);

} // namespace utils::sfml
