#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "../src/Data/RegionDatabase.h"

BOOST_AUTO_TEST_SUITE(RegionsTests)

BOOST_AUTO_TEST_CASE(compare_regions)
{
    auto continent = std::make_shared<gs::Continent>("Norh America");
    continent->addChild<gs::Country>("USA");
    //app::MyClass test("test");
    //BOOST_TEST(test.appendIt("test") == "test test");
    //BOOST_TEST(test.text() == "test");
}

BOOST_AUTO_TEST_SUITE_END()    
