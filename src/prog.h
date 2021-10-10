#ifndef PROG_H
#define PROG_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

struct Prog
{
    bool running;

    SDL_Window* window;
    SDL_Renderer* rend;

    TTF_Font* font;
};

struct Prog* prog_alloc();
void prog_free(struct Prog* self);

void prog_mainloop(struct Prog* self);

#endif

