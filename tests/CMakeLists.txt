message(STATUS "Building tests...")
find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

set(SRC_TEST parser.cpp)

add_executable(tests ${SRC_TEST})
target_link_libraries(tests
        PUBLIC
        Catch2::Catch2WithMain
        cura-formulae-engine
        zeus::expected
        foonathan::lexy
        range-v3::range-v3
        spdlog::spdlog
)

if (MSVC)
    target_compile_options(tests PRIVATE /bigobj)
endif ()

catch_discover_tests(tests
        TEST_PREFIX
        "unittests."
        OUTPUT_DIR
        .
        OUTPUT_PREFIX
        "unittests."
        OUTPUT_SUFFIX
        .xml)