#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <CLI/CLI.hpp>

#include "wol/mac_address.hpp"
#include "wol/magic_packet.hpp"
#include "wol/udp_sender.hpp"

int main(int argc, char* argv[]) {
    CLI::App app{
        "Wake-on-LAN utility\n"
        "Sends a WoL magic packet to power on a remote machine."
    };

    std::string mac_str;
    std::string address = "255.255.255.255";
    std::uint16_t port = 9;
    int repeat = 1;
    int interval_ms = 100;

    app.add_option(
           "--mac",
           mac_str,
           "Target MAC address (AA:BB:CC:DD:EE:FF)"
       )
       ->required();

    app.add_option(
           "--address",
           address,
           "Destination IPv4 address (broadcast or unicast)"
       )
       ->check(CLI::ValidIPV4);

    app.add_option(
           "--port",
           port,
           "UDP destination port"
       )
       ->check(CLI::Range(1, 65535));

    app.add_option(
           "--repeat",
           repeat,
           "Number of packets to send"
       )
       ->check(CLI::Range(1, 100));

    app.add_option(
           "--interval",
           interval_ms,
           "Interval between packets in milliseconds"
       )
       ->check(CLI::Range(0, 10'000));

    try {
        app.parse(argc, argv);

        // Parse and validate MAC address
        auto mac = MacAddress::from_string(mac_str);
        if (!mac) {
            std::cerr << "Invalid MAC address format\n";
            return EXIT_FAILURE;
        }

        MagicPacket packet(*mac);
        UdpSender sender;

        for (int i = 0; i < repeat; ++i) {
            sender.send(packet.data(), address, port);

            if (i + 1 < repeat && interval_ms > 0) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(interval_ms)
                );
            }
        }

        return EXIT_SUCCESS;
    }
    catch (const CLI::ParseError& e) {
        return app.exit(e);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
