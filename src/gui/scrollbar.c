#include "scrollbar.h"


struct Scrollbar* scrollbar_alloc(SDL_Rect rect)
{
    struct Scrollbar* self = malloc(sizeof(struct Scrollbar));
    self->rect = rect;

    self->total_units = 0;
    self->bar_len_units = 0;
    self->bar_top_units = 0;
    self->px_per_unit = 0.f;

    self->held = false;

    return self;
}


void scrollbar_free(struct Scrollbar* self)
{
    free(self);
}


void scrollbar_render(struct Scrollbar* self, SDL_Renderer* rend)
{
    SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
    SDL_RenderFillRect(rend, &self->rect);

    SDL_Rect bar = {
        .x = self->rect.x,
        .y = self->rect.y + self->bar_top_units * self->px_per_unit,
        .w = self->rect.w,
        .h = self->bar_len_units * self->px_per_unit
    };

    SDL_SetRenderDrawColor(rend, 90, 90, 90, 255);
    SDL_RenderFillRect(rend, &bar);
}


void scrollbar_update(struct Scrollbar* self, int total_units, int rows_visible)
{
    self->total_units = total_units;
    self->px_per_unit = (float)self->rect.h / (float)total_units;
    self->bar_len_units = rows_visible;
}


void scrollbar_scroll(struct Scrollbar* self, int y)
{
    self->bar_top_units += y;

    if (self->bar_top_units < 0)
        self->bar_top_units = 0;

    if (self->bar_top_units + self->bar_len_units > self->total_units)
        self->bar_top_units = self->total_units - self->bar_len_units;
}

