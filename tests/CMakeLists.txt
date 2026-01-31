include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

add_executable(wol-tests
    test_mac_address.cpp
    test_magic_packet.cpp
)

target_link_libraries(wol-tests
    PRIVATE
        wol
        Catch2::Catch2WithMain
)

add_test(
    NAME wol-tests
    COMMAND wol-tests
)
