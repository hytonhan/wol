#include "wol/udp_sender.hpp"

#include <cstring>
#include <stdexcept>

#ifdef _WIN32
    #define NOMINMAX
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#ifdef _WIN32
struct WsaContext {
    WsaContext() {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
    }
    ~WsaContext() {
        WSACleanup();
    }
};

WsaContext& wsa_context() {
    static WsaContext ctx;
    return ctx;
}
#endif

UdpSender::UdpSender() {
#ifdef _WIN32
    (void)wsa_context();
#endif

    socket_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ == invalid_socket) {
        throw std::runtime_error("Failed to create UDP socket");
    }

    int enable = 1;
    if (setsockopt(
            socket_,
            SOL_SOCKET,
            SO_BROADCAST,
#ifdef _WIN32
            reinterpret_cast<const char*>(&enable),
#else
            &enable,
#endif
            sizeof(enable)) < 0) {
        close();
        throw std::runtime_error("Failed to enable broadcast on socket");
    }
}

UdpSender::~UdpSender() {
    close();
}

UdpSender::UdpSender(UdpSender&& other) noexcept
    : socket_(other.socket_) {
    other.socket_ = invalid_socket;
}

UdpSender& UdpSender::operator=(UdpSender&& other) noexcept {
    if (this != &other) {
        close();
        socket_ = other.socket_;
        other.socket_ = invalid_socket;
    }
    return *this;
}

void UdpSender::close() noexcept {
    if (socket_ != invalid_socket) {
#ifdef _WIN32
        closesocket(socket_);
#else
        ::close(socket_);
#endif
        socket_ = invalid_socket;
    }
}

void UdpSender::send(std::span<const std::uint8_t> payload,
                     std::string_view address,
                     std::uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, address.data(), &addr.sin_addr) != 1) {
        throw std::invalid_argument("Invalid IPv4 address");
    }

    const auto sent = ::sendto(
        socket_,
#ifdef _WIN32
        reinterpret_cast<const char*>(payload.data()),
#else
        payload.data(),
#endif
        static_cast<int>(payload.size()),
        0,
        reinterpret_cast<const sockaddr*>(&addr),
        sizeof(addr));

    if (sent < 0 || static_cast<std::size_t>(sent) != payload.size()) {
        throw std::runtime_error("Failed to send UDP packet");
    }
}
