#ifndef FRONTEND_H
#define FRONTEND_H

#include "gui/textbox.h"
#include <SDL.h>

struct Frontend
{
    SDL_Window* window;
    SDL_Renderer* rend;

    TTF_Font* font;

    struct Textbox* main_textbox;
    struct Textbox* selected_textbox;
};

struct Frontend* frontend_alloc(SDL_Window* window, SDL_Renderer* rend);
void frontend_free(struct Frontend* self);

void frontend_render(struct Frontend* self);

#endif

