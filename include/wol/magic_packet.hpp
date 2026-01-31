#pragma once

#include <span>
#include <cstdint>
#include "mac_address.hpp"

class MagicPacket {
public:
    static constexpr int SyncSize = 6;
    static constexpr int RepeatCount = 16;
    static constexpr int PacketSize = SyncSize + (RepeatCount * MacAddress::macLength);

    explicit MagicPacket(const MacAddress& mac);

    std::span<const std::uint8_t> data() const noexcept {
        return buffer_;
    }

private:
    std::array<std::uint8_t, PacketSize> buffer_;
};
