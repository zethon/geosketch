#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "../src/Data/RegionDatabase.h"

namespace nl = nlohmann;

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

BOOST_AUTO_TEST_CASE(region_hash)
{
    auto nacontinent = std::make_shared<gs::Continent>("North America");
    auto usa_na = nacontinent->addChild<gs::Country>("USA");

    auto sacontinent = std::make_shared<gs::Continent>("South America");
    auto usa_sa = sacontinent->addChild<gs::Country>("USA");

    auto afcontinent = std::make_shared<gs::Continent>("Africa");
    auto usa_af = afcontinent->addChild<gs::Country>("USA");

    BOOST_TEST(std::hash<gs::Region>{}(*(nacontinent->children()[0])) == std::hash<gs::Region>{}(*usa_na));
    BOOST_TEST(std::hash<gs::Region>{}(*usa_sa) == std::hash<gs::Region>{}(*(sacontinent->children()[0])));
    BOOST_TEST(std::hash<gs::Region>{}(*usa_af) == std::hash<gs::Region>{}(*(afcontinent->children()[0])));
}

BOOST_AUTO_TEST_CASE(json_read_write)
{
    constexpr auto json = R"(
{
    "type" : "country",
    "name" : "Canada",
    "capital" : "Ottawa",
    "area" : 9984670,
    "population" : 36624199,
    "currency" : "Canadian Dollar",
    "language" : "English, French",
    "flag" : "canada.png",
    "factoids":
    [
        "Canada has the longest coastline of any country in the world.",
        "Canada has the longest highway in the world"
    ]
}
    )";

    nl::json j = nl::json::parse(json);
    auto type = j["type"].get<gs::RegionType>();
    BOOST_TEST((type == gs::RegionType::COUNTRY));

    auto region = j.template get<gs::Country>();
    BOOST_TEST(region.name() == "Canada");
    BOOST_TEST(region.type() == gs::RegionType::COUNTRY);
}

BOOST_AUTO_TEST_SUITE_END()
