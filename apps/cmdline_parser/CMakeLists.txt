add_executable(cmdline_parser
        cmdline_parser.cpp
)
if (MSVC)
    target_compile_options(cmdline_parser PRIVATE /bigobj)
endif ()

enable_sanitizers(cmdline_parser)
if (${EXTENSIVE_WARNINGS})
    set_project_warnings(cmdline_parser)
endif ()

target_link_libraries(cmdline_parser PUBLIC cura-formulae-engine foonathan::lexy range-v3::range-v3)

