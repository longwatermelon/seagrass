project(gui)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

add_library(gui
    text.h
    text.c
)

target_link_libraries(gui PUBLIC
    SDL2::SDL2 SDL2::SDL2main
    SDL2::SDL2_image
    SDL2::SDL2_ttf
)

