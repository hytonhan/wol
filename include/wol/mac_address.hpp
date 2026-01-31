#pragma once

#include <string_view>
#include <cstdint>
#include <array>
#include <optional>

class MacAddress {
public:
    static constexpr std::size_t macLength = 6;

    static std::optional<MacAddress> from_string(std::string_view);
    const std::array<std::uint8_t, macLength>& bytes() const noexcept {
        return bytes_;
    }

private:
    explicit constexpr MacAddress(std::array<std::uint8_t, macLength> bytes) noexcept
        : bytes_(bytes) {}

    std::array<std::uint8_t, macLength> bytes_;
};