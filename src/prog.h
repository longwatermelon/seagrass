#ifndef PROG_H
#define PROG_H

#include <stdbool.h>
#include <SDL.h>

struct Prog
{
    bool running;

    SDL_Window* window;
    SDL_Renderer* rend;
};

struct Prog* prog_alloc();
void prog_free(struct Prog* self);

void prog_mainloop(struct Prog* self);

#endif

