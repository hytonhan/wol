#include <catch2/catch_test_macros.hpp>
#include "wol/mac_address.hpp"

TEST_CASE("MacAddress parses colon-separated format") {
    auto mac = MacAddress::from_string("AA:BB:CC:DD:EE:FF");
    REQUIRE(mac.has_value());

    const auto& bytes = mac.value().bytes();
    REQUIRE(bytes[0] == 0xAA);
    REQUIRE(bytes[1] == 0xBB);
    REQUIRE(bytes[2] == 0xCC);
    REQUIRE(bytes[3] == 0xDD);
    REQUIRE(bytes[4] == 0xEE);
    REQUIRE(bytes[5] == 0xFF);
}

TEST_CASE("MacAddress parses dash-separated format") {
    auto mac = MacAddress::from_string("aa-bb-cc-dd-ee-ff");
    REQUIRE(mac.has_value());

    REQUIRE(mac->bytes()[0] == 0xAA);
}

TEST_CASE("MacAddress parses dot-separated format") {
    auto mac = MacAddress::from_string("aabb.ccdd.eeff");
    REQUIRE(mac.has_value());

    REQUIRE(mac->bytes()[0] == 0xAA);
}

TEST_CASE("MacAddress parses raw hex format") {
    auto mac = MacAddress::from_string("AABBCCDDEEFF");
    REQUIRE(mac.has_value());

    REQUIRE(mac->bytes()[5] == 0xFF);
}

TEST_CASE("MacAddress rejects too short input") {
    REQUIRE_FALSE(MacAddress::from_string("AA:BB:CC"));
}

TEST_CASE("MacAddress rejects invalid characters") {
    REQUIRE_FALSE(MacAddress::from_string("GG:BB:CC:DD:EE:FF"));
}

TEST_CASE("MacAddress rejects too long input") {
    REQUIRE_FALSE(MacAddress::from_string("AA:BB:CC:DD:EE:FFZZ"));
}
