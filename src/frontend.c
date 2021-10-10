#include "frontend.h"


struct Frontend* frontend_alloc(SDL_Window* window, SDL_Renderer* rend)
{
    struct Frontend* self = malloc(sizeof(struct Frontend));
    self->window = window;
    self->rend = rend;

    self->font = TTF_OpenFont("res/gfx/font.ttf", 16);
    
    int wx, wy;
    SDL_GetWindowSize(window, &wx, &wy);

    self->main_textbox = textbox_alloc((SDL_Rect){ 300, 200, wx - 300, wy - 200 }, rend, self->font, (SDL_Color){ 50, 50, 50 });
    self->selected_textbox = 0;

    return self;
}


void frontend_free(struct Frontend* self)
{
    textbox_free(self->main_textbox);
    TTF_CloseFont(self->font);

    free(self);
}


void frontend_render(struct Frontend* self)
{
    SDL_GetWindowSize(self->window, &self->main_textbox->rect.w, &self->main_textbox->rect.h);
    self->main_textbox->rect.w -= self->main_textbox->rect.x;
    self->main_textbox->rect.h -= self->main_textbox->rect.y;

    textbox_render(self->main_textbox, self->rend);
}

