#include "wol/magic_packet.hpp"
//#include <algorithm>

MagicPacket::MagicPacket(const MacAddress& mac) {
    buffer_.fill(0xFF);

    const auto& mac_bytes = mac.bytes();

    // Copy MAC address 16 times after sync stream
    auto it = buffer_.begin() + SyncSize;
    for (int i = 0; i < RepeatCount; ++i) {
        it = std::copy(mac_bytes.begin(), mac_bytes.end(), it);
    }
}
