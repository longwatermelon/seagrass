#include "prog.h"
#include "events.h"


struct Prog* prog_alloc()
{
    struct Prog* self = malloc(sizeof(struct Prog));
    self->running = true;

    self->window = SDL_CreateWindow("Seagrass", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    self->rend = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    return self;
}


void prog_free(struct Prog* self)
{
    SDL_DestroyRenderer(self->rend);
    SDL_DestroyWindow(self->window);

    free(self);
}


void prog_mainloop(struct Prog* self)
{
    SDL_Event evt;

    while (self->running)
    {
        events_base(self, &evt);

        SDL_RenderClear(self->rend);

        SDL_SetRenderDrawColor(self->rend, 0, 0, 0, 255);
        SDL_RenderPresent(self->rend);
    }
}

