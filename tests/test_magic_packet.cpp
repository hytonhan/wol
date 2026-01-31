#include <catch2/catch_test_macros.hpp>
#include "wol/magic_packet.hpp"


TEST_CASE("MagicPacket has correct size") {
    auto mac = MacAddress::from_string("AA:BB:CC:DD:EE:FF");
    REQUIRE(mac);

    MagicPacket packet(*mac);
    REQUIRE(packet.data().size() == MagicPacket::PacketSize);
}

TEST_CASE("MagicPacket starts with sync stream") {
    auto mac = MacAddress::from_string("01:02:03:04:05:06");
    REQUIRE(mac);

    MagicPacket packet(*mac);
    auto data = packet.data();

    for (int i = 0; i < MagicPacket::SyncSize; ++i) {
        REQUIRE(data[i] == 0xFF);
    }
}

TEST_CASE("MagicPacket repeats MAC address 16 times") {
    auto mac = MacAddress::from_string("01:02:03:04:05:06");
    REQUIRE(mac);

    MagicPacket packet(*mac);
    auto data = packet.data();

    int offset = MagicPacket::SyncSize;

    for (int repeat = 0; repeat < MagicPacket::RepeatCount; ++repeat) {
        for (int byte = 0; byte < MacAddress::macLength; ++byte) {
            REQUIRE(data[offset + byte] == mac->bytes()[byte]);
        }
        offset += MacAddress::macLength;
    }
}
