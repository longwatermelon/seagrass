#include "textbox.h"


struct Textbox* textbox_alloc(SDL_Point pos, SDL_Renderer* rend, TTF_Font* font)
{
    struct Textbox* self = malloc(sizeof(struct Textbox));
    self->pos = pos;

    self->text = text_alloc(rend, pos, "", font, (SDL_Color){ 255, 255, 255 });

    return self;
}


void textbox_free(struct Textbox* self)
{
    text_free(self->text);
    free(self);
}


void textbox_render(struct Textbox* self, SDL_Renderer* rend)
{
    text_render(self->text, rend);
}

