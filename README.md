# Wake-on-LAN (WoL) – Modern C++ Library & CLI

A cross-platform Wake-on-LAN implementation written in modern C++20, consisting of a reusable library and a professional command-line interface.

This project demonstrates:

Low-level network protocol understanding

Cross-platform socket programming (POSIX / WinSock)

Idiomatic modern C++ design (RAII, strong typing, zero-cost abstractions)

## Features

- Byte-accurate Wake-on-LAN magic packet implementation
- Cross-platform UDP sender (Linux / macOS / Windows)
- Clean separation between library and CLI
- Strongly-typed MAC address parsing and validation
- Deterministic, allocation-free packet construction
- Professional CLI built with CLI11
- Unit tests using Catch2

## Project structure

```
wol/
├── include/
│   └── wol/
│       ├── mac_address.hpp
│       ├── magic_packet.hpp
│       └── udp_sender.hpp
├── src/
│   ├── mac_address.cpp
│   ├── magic_packet.cpp
│   ├── udp_sender.cpp
│   └── main.cpp
├── tests/
│   ├── test_mac_address.cpp
│   ├── test_magic_packet.cpp
│   └── CMakeLists.txt
├── CMakeLists.txt
└── README.md
```

## Wake-on-LAN Protocol Overview

Wake-on-LAN works by sending a magic packet over the network.
The target machine’s NIC listens for this packet even while the system is powered off.

Magic Packet Format
```
[6 bytes]   FF FF FF FF FF FF
[16 times]  Target MAC address (6 bytes each)
```

Total packet size: 102 bytes
- Transport: UDP
- Common ports: 7, 9 (port is ignored by the NIC)
- Destination: broadcast or directed broadcast address

## Modern C++ Practices
- C++20
- std::array, std::span, std::optional
- RAII for all OS resources
- No raw new / delete
- No global state
- No platform headers in public interfaces

## Cross-Platform Networking
- POSIX sockets on Unix-like systems
- WinSock on Windows
- Platform-specific code isolated to .cpp files
- Explicit SO_BROADCAST configuration

## Building

### Requirements
- CMake ≥ 3.20
- C++20 compiler
- Internet access (for FetchContent dependencies)

### Configure and Build
```bash
cmake -S . -B build
cmake --build build
```
The CLI executable will be produced as:
```
build/wol
```

## Running the CLI

### Basic Usage

```bash 
wol --mac AA:BB:CC:DD:EE:FF
```

### Advanced Example

```bash
wol \
  --mac AA:BB:CC:DD:EE:FF \
  --address 192.168.1.255 \
  --port 9 \
  --repeat 5 \
  --interval 200
```

### Options

| Option       | Description                   | Default           |
| ------------ | ----------------------------- | ----------------- |
| `--mac`      | Target MAC address (required) | —                 |
| `--address`  | Destination IPv4 address      | `255.255.255.255` |
| `--port`     | UDP port                      | `9`               |
| `--repeat`   | Number of packets to send     | `1`               |
| `--interval` | Interval between packets (ms) | `100`             |

## Running Tests

Tests are enabled by default.
```bash
ctest --test-dir build --output-on-failure
```

Or run the test binary directly:
```
./build/tests/wol-tests
```

## Notes & Limitations
- IPv4-only (sufficient for most WoL use cases)
- UDP is unreliable by nature — repeat sends improve success rates
- BIOS/UEFI and NIC must support Wake-on-LAN
- Routers may block directed broadcasts

## Future Improvements
- IPv6 Wake-on-LAN support
- Secure relay service for waking hosts across networks
- Network discovery (ARP scan)
- CI pipeline (GitHub Actions)
