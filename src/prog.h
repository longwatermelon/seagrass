#ifndef PROG_H
#define PROG_H

#include "gui/scrollbar.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

struct Prog
{
    bool running;

    SDL_Window* window;
    SDL_Renderer* rend;

    TTF_Font* font;

    struct Textbox* main_textbox;
    struct Scrollbar* main_scrollbar;

    struct Textbox* selected_textbox;
};

struct Prog* prog_alloc();
void prog_free(struct Prog* self);

void prog_mainloop(struct Prog* self);
void prog_render(struct Prog* self);

void prog_mainloop_textbox(struct Prog* self);
void prog_mainloop_scrollbar(struct Prog* self);

#endif

