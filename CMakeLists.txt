cmake_minimum_required(VERSION 3.20)

project(
    wol
    VERSION 0.1.0
    DESCRIPTION "Using Wake-on-LAN protocol to remote power On a PC"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile_commands.json (clang-tidy, clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(wol
    src/mac_address.cpp
    src/magic_packet.cpp
    src/udp_sender.cpp
)
# add_library(wol::wol ALIAS wol)
target_include_directories(wol
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (WIN32)
    target_link_libraries(wol PRIVATE ws2_32)
endif()




add_executable(wol-cli
    src/main.cpp
)

target_link_libraries(wol-cli
    PRIVATE
        wol
)

set_target_properties(wol-cli PROPERTIES OUTPUT_NAME wol)


enable_testing()
add_subdirectory(tests)


include(GNUInstallDirs)

install(
    TARGETS wol wol-cli
    EXPORT wolTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT wolTargets
    NAMESPACE wol::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wol
)