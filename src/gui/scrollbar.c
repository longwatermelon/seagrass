#include "scrollbar.h"
#include "utils.h"


struct Scrollbar* scrollbar_alloc(SDL_Rect rect)
{
    struct Scrollbar* self = malloc(sizeof(struct Scrollbar));
    self->rect = rect;

    self->total_units = 0;
    self->bar_len_units = 0;
    self->bar_top_units = 0;
    self->px_per_unit = 0.f;

    self->held = false;
    self->mouse_dist = 0;

    return self;
}


void scrollbar_free(struct Scrollbar* self)
{
    free(self);
}


void scrollbar_render(struct Scrollbar* self, SDL_Renderer* rend, int mx, int my)
{
    SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
    SDL_RenderFillRect(rend, &self->rect);

    SDL_Rect bar = {
        .x = self->rect.x,
        .y = self->rect.y + self->bar_top_units * self->px_per_unit,
        .w = self->rect.w,
        .h = self->bar_len_units * self->px_per_unit
    };

    int col = 90;

    if (self->held)
        col -= 30;
    else if (utils_p_in_rect((SDL_Point){ .x = mx, .y = my }, bar))
        col += 30;

    SDL_SetRenderDrawColor(rend, col, col, col, 255);
    SDL_RenderFillRect(rend, &bar);
}


void scrollbar_update_units(struct Scrollbar* self, int total_units, int bar_len_units)
{
    self->total_units = total_units;
    self->px_per_unit = (float)self->rect.h / (float)total_units;
    self->bar_len_units = bar_len_units;
}


void scrollbar_follow_mouse(struct Scrollbar* self)
{
    int my;
    SDL_GetMouseState(0, &my);

    my -= self->rect.y;

    if (abs(my - self->mouse_dist) >= self->px_per_unit)
    {
        scrollbar_scroll(self, roundf((float)(my - self->mouse_dist) / self->px_per_unit));
        self->mouse_dist = my;
    }
}


void scrollbar_scroll(struct Scrollbar* self, int y)
{
    self->bar_top_units += y;

    if (self->bar_top_units < 0)
        self->bar_top_units = 0;

    if (self->bar_top_units + self->bar_len_units > self->total_units)
        self->bar_top_units = self->total_units - self->bar_len_units;
}

