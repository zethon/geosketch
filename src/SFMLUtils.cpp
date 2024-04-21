#include <cmath>
#include <vector>
#include <cstdlib>

#include <SFML/System/Vector2.hpp>

#include "SFMLUtils.h"

namespace utils::sfml
{

template<class V>
bool AreAdjacent(const V& first, const V& second)
{
    return (std::abs(static_cast<float>(first.x - second.x)) < 1.0f)
        || (std::abs(static_cast<float>(first.y - second.y)) < 1.0f);
}

template<class V>
std::vector<V> GetIntersected(const V& first, const V& second)
{
    return {};
//   std::vector<V> intersecting_points;

//   auto x1 = first.x;
//   auto y1 = first.y;
//   auto x2 = second.x;
//   auto y2 = second.y;

//   // Calculate the slope of the line
//   double slope;
//   if (x2 - x1 != 0) 
//   {
//       slope = static_cast<double>(y2 - y1) / (x2 - x1);
//   } 
//   else 
//   {
//       slope = std::numeric_limits<double>::infinity();  // Vertical line
//   }

//   // Determine the range of x values
//   auto min_x = std::min(x1, x2);
//   auto max_x = std::max(x1, x2);

//     // Iterate over the x values within the range
//     for (auto x = min_x; x <= max_x; ++x) 
//     {
//         // Calculate the corresponding y value
//         decltype(x) y = 0;
//         if (slope != std::numeric_limits<double>::infinity()) 
//         {
//             y = static_cast<int>(y1 + slope * (x - x1));
//         } else 
//         {
//             y = std::min(y1, y2) + static_cast<decltype(y1)>(y2 - y1) * (x == x1);  // Handle vertical line
//         }

//         // Add the point (x, y) to the list of intersecting points
//         intersecting_points.push_back({x, y});
//     }

//     return intersecting_points;
  // // Calculate the difference between p1 and p2.
  //   const float xdiff = second.x - first.y;
  //   const float ydiff = second.y - first.y;

  // // Find the number of steps between p1 and p2.
  // const auto steps = std::max(std::abs(xdiff), std::abs(ydiff));

  // // Create a list of points between p1 and p2.
  // std::vector<V> points;
  // for (int i = 0; i <= steps; i++)
  // {
  //     const decltype(first.x) xpt = first.x + (xdiff / steps);
  //     const decltype(first.y) ypt = first.y + (ydiff / steps);
  //     points.emplace_back(xpt, ypt);
  // }

  // return points;
}

template bool AreAdjacent(const sf::Vector2u& first, const sf::Vector2u& second);
template bool AreAdjacent(const sf::Vector2i& first, const sf::Vector2i& second);
template bool AreAdjacent(const sf::Vector2f& first, const sf::Vector2f& second);

template std::vector<sf::Vector2u> GetIntersected(const sf::Vector2u&, const sf::Vector2u&);
template std::vector<sf::Vector2i> GetIntersected(const sf::Vector2i&, const sf::Vector2i&);
template std::vector<sf::Vector2f> GetIntersected(const sf::Vector2f&, const sf::Vector2f&);

} // namespace utils::sfml
