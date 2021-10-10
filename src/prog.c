#include "prog.h"
#include "events.h"
#include "gui/text.h"


struct Prog* prog_alloc()
{
    struct Prog* self = malloc(sizeof(struct Prog));
    self->running = true;

    self->window = SDL_CreateWindow("Seagrass", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    self->rend = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    self->font = TTF_OpenFont("res/gfx/font.ttf", 16);

    return self;
}


void prog_free(struct Prog* self)
{
    TTF_CloseFont(self->font);

    SDL_DestroyRenderer(self->rend);
    SDL_DestroyWindow(self->window);

    free(self);
}


void prog_mainloop(struct Prog* self)
{
    SDL_Event evt;

    struct Text* t = text_alloc(self->rend, (SDL_Point){ 100, 100 }, "lawlwla\nowo", self->font, (SDL_Color){ 255, 255, 255 });

    while (self->running)
    {
        events_base(self, &evt);

        SDL_RenderClear(self->rend);

        text_render(t, self->rend);

        SDL_SetRenderDrawColor(self->rend, 0, 0, 0, 255);
        SDL_RenderPresent(self->rend);
    }

    text_free(t);
}

