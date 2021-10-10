cmake_minimum_required(VERSION 3.20.5)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(seagrass)

add_subdirectory(src/gui)

add_executable(seagrass
    src/main.c
    src/prog.h
    src/prog.c
    src/events.h
    src/events.c
    src/frontend.h
    src/frontend.c
)

target_compile_options(seagrass PRIVATE -ggdb)
target_link_libraries(seagrass PRIVATE gui)

