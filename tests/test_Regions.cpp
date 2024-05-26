#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "../src/Data/RegionDatabase.h"


BOOST_AUTO_TEST_SUITE(RegionsTests)

BOOST_AUTO_TEST_CASE(compare_regions)
{
    auto nacontinent = std::make_shared<gs::Continent>("North America");
    auto usa_na = nacontinent->addChild<gs::Country>("USA");
    auto duplicate = std::make_shared<gs::Continent>("North America");

    BOOST_TEST(*nacontinent != *nacontinent->children()[0]);
    BOOST_TEST(*nacontinent != *duplicate);

    auto sacontinent = std::make_shared<gs::Continent>("South America");
    auto usa_sa = sacontinent->addChild<gs::Country>("USA");

    BOOST_TEST(*usa_na != *usa_sa);

    BOOST_TEST(*nacontinent == *nacontinent);
    BOOST_TEST(*usa_na == *(nacontinent->children()[0]));
}

BOOST_AUTO_TEST_CASE(complex_compare_regions_test)
{
    // A: NORTH AMERICA -> USA -> WASHINGTON
    // B: SOUTH AMERICA -> USA -> WASHINGTON
    // A == B -> FALSE

    auto nacontinent = std::make_shared<gs::Continent>("North America");
    auto usa_na = nacontinent->addChild<gs::Country>("USA");

    auto sacontinent = std::make_shared<gs::Continent>("South America");
    auto usa_sa = sacontinent->addChild<gs::Country>("USA");

    auto afcontinent = std::make_shared<gs::Continent>("Africa");
    auto usa_af = afcontinent->addChild<gs::Country>("USA");

    BOOST_TEST(*nacontinent == *nacontinent);
    BOOST_TEST(*usa_af == *usa_af);

    BOOST_TEST(*usa_af != *usa_na);
    BOOST_TEST(*usa_na != *usa_sa);
    BOOST_TEST(*usa_sa != *usa_af);
}

BOOST_AUTO_TEST_SUITE_END()
