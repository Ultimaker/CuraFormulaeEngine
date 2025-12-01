cmake_minimum_required(VERSION 3.26)
project(cura-formulae-engine)

find_package(standardprojectsettings REQUIRED)
option(ENABLE_TESTS "Build with unit test" ON)
option(EXTENSIVE_WARNINGS "Build with all warnings" ON)
option(ENABLE_APPS "Build apps example" ON)

find_package(spdlog REQUIRED)
find_package(lexy REQUIRED)
find_package(zeus_expected REQUIRED)
find_package(range-v3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED CURA_FORMULA_VERSION_VERSION)
    message(FATAL_ERROR "CURA_FORMULA_VERSION_VERSION is not defined!")
endif ()
message(STATUS "Configuring CuraFormuleaEngine version: ${CURA_FORMULA_VERSION_VERSION}")

set(CURA_FORMULAE_ENGINE__SRC
    src/eval.cpp
    src/env/abs.cpp
    src/env/map.cpp
    src/env/math_ceil.cpp
    src/env/math_cos.cpp
    src/env/math_degrees.cpp
    src/env/math_floor.cpp
    src/env/math_log.cpp
    src/env/math_radians.cpp
    src/env/math_sin.cpp
    src/env/math_sqrt.cpp
    src/env/math_tan.cpp
    src/env/max.cpp
    src/env/min.cpp
    src/env/round.cpp
    src/env/str.cpp
    src/env/sum.cpp
    src/env/all.cpp
    src/env/any.cpp
    src/env/float_fn.cpp
    src/env/int_fn.cpp
    src/env/len.cpp
    src/env/math_atan.cpp
    src/env/env.cpp
    src/ast/ast.cpp
    src/ast/unary_expr/unary_expr.cpp
    src/ast/unary_expr/neg_expr.cpp
    src/ast/unary_expr/not_expr.cpp
    src/ast/comp_chain_expr.cpp
    src/ast/condition_expr.cpp
    src/ast/expr_ptr.cpp
    src/ast/fn_application_expr.cpp
    src/ast/index_expr.cpp
    src/ast/list_comprehension_expr.cpp
    src/ast/list_expr.cpp
    src/ast/slice_expr.cpp
    src/ast/tuple_expr.cpp
    src/ast/variable_expr.cpp
    src/parser/parser.cpp
    src/ast/binary_expr/binary_expr.cpp
    src/ast/binary_expr/add_expr.cpp
    src/ast/binary_expr/sub_expr.cpp
    src/ast/binary_expr/div_expr.cpp
    src/ast/binary_expr/mod_expr.cpp
    src/ast/binary_expr/mul_expr.cpp
    src/ast/binary_expr/or_expr.cpp
    src/ast/binary_expr/pow_expr.cpp
    src/ast/binary_expr/and_expr.cpp
        src/ast/primary_expr/string_expr.cpp
)

add_library(cura-formulae-engine SHARED ${CURA_FORMULAE_ENGINE__SRC})

target_link_libraries(cura-formulae-engine
    PUBLIC
    fmt::fmt
    spdlog::spdlog
    foonathan::lexy
    zeus::expected
    range-v3::range-v3
)

target_compile_definitions(cura-formulae-engine
        INTERFACE
        CURA_FORMULA_VERSION_VERSION="${CURA_FORMULA_VERSION_VERSION}"
)

target_include_directories(
        cura-formulae-engine
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (ENABLE_APPS)
    add_subdirectory(apps)
endif ()

if (${EXTENSIVE_WARNINGS})
    set_project_warnings(cura-formulae-engine)
endif ()

if (ENABLE_TESTS)
    add_subdirectory(tests)
endif ()
