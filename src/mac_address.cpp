#include "wol/mac_address.hpp"
#include <cstdint>
#include <iterator>
#include <optional>


constexpr int hex_value(char c) noexcept {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

std::optional<MacAddress> MacAddress::from_string(std::string_view str) {
    std::array<std::uint8_t, macLength> bytes{};
    std::size_t byte_index = 0;

    auto it = str.begin();
    auto end = str.end();

    while (it != end && byte_index < macLength)
    {
        if (*it == ':' || *it == '-') {
            ++it;
            continue;
        }

        if (std::distance(it, end) < 2) {
            return std::nullopt;
        }

        int8_t high = hex_value(*it); ++it;
        int8_t low = hex_value(*it); ++it;

        if (high < 0 || low < 0)
        {
            return std::nullopt;
        }

        bytes[byte_index] = static_cast<uint8_t>((high << 4) | low); 
        ++byte_index;
    }

    if (byte_index != macLength) {
        return std::nullopt;
    }
    if (it != end)  {
        return std::nullopt;
    }

    return MacAddress{bytes};
}
