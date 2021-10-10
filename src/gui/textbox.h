#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "text.h"

struct Textbox
{
    SDL_Point pos;
    struct Text* text;
};

struct Textbox* textbox_alloc(SDL_Point pos, SDL_Renderer* rend, TTF_Font* font);
void textbox_free(struct Textbox* self);

void textbox_render(struct Textbox* self, SDL_Renderer* rend);

#endif

