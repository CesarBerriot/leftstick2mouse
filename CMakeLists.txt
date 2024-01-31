cmake_minimum_required(VERSION 3.27)
project(leftstick2mouse C)

set(CMAKE_C_STANDARD 11)

add_executable(leftstick2mouse WIN32
        src/main.c
        src/logic/logic.c
        src/logic/logic.h
        src/ui/ui.c
        src/ui/ui.h)

target_link_libraries(leftstick2mouse PRIVATE Xinput1_4.dll gdi32.lib user32.lib)

target_include_directories(leftstick2mouse PRIVATE
        ${PROJECT_SOURCE_DIR}/libs/luigi/include
        ${PROJECT_SOURCE_DIR}/src)

target_compile_options(leftstick2mouse PRIVATE
        -Wno-incompatible-pointer-types)