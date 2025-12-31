cmake_minimum_required(VERSION 3.31)

project(New2026_Year)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(New2026_Year
    src/main.c
    src/libraries/string_bfo/String_array_struct.c
    src/libraries/string_bfo/String_struct.c
    src/libraries/string_bfo/String_struct_formatting.c
    src/libraries/string_bfo/String_struct.h
    src/libraries/terminal_bfo/colors_bfo/bins.c
    src/libraries/terminal_bfo/colors_bfo/bins.h
    src/libraries/terminal_bfo/colors_bfo/colors.c
    src/libraries/terminal_bfo/colors_bfo/colors.h
    src/libraries/terminal_bfo/progress_bar/progres_bar.c
    src/libraries/terminal_bfo/progress_bar/progres_bar.h
    src/libraries/terminal_bfo/terminal_funcs.h
)

target_link_libraries(New2026_Year
    m
)
