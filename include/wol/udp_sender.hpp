#pragma once

#include <cstdint>
#include <span>
#include <string_view>

class UdpSender {
public:
    UdpSender();
    ~UdpSender();

    UdpSender(const UdpSender&) = delete;
    UdpSender& operator=(const UdpSender&) = delete;
    UdpSender(UdpSender&&) noexcept;
    UdpSender& operator=(UdpSender&&) noexcept;

    void send(std::span<const std::uint8_t> payload,
              std::string_view broadcastAddress,
              std::uint16_t port);

private:
#ifdef _WIN32
    using socket_type = std::uintptr_t;
    static constexpr socket_type invalid_socket = static_cast<socket_type>(~0);
#else
    using socket_type = int;
    static constexpr socket_type invalid_socket = -1;
#endif

    socket_type socket_{invalid_socket};

    void close() noexcept;
};
