#include "prog.h"
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    struct Prog* p = prog_alloc();
    prog_mainloop(p);
    prog_free(p);

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    return 0;
}

