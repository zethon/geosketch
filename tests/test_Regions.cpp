#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "../src/Data/RegionDatabase.h"


BOOST_AUTO_TEST_SUITE(RegionsTests)

BOOST_AUTO_TEST_CASE(compare_regions)
{
    auto continent = std::make_shared<gs::Continent>("North America");
    continent->addChild<gs::Country>("USA");

    auto duplicate = std::make_shared<gs::Continent>("North America");

    BOOST_TEST(*continent != *continent->children()[0]);
    BOOST_TEST(*continent == *duplicate);
    BOOST_TEST(*continent == *continent->children()[0]);
}

BOOST_AUTO_TEST_SUITE_END()
