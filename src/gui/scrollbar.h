#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <stdbool.h>
#include <SDL2/SDL.h>

struct Scrollbar
{
    SDL_Rect rect;

    int total_units;
    int bar_top_units, bar_len_units;

    float px_per_unit;

    // Dragged by mouse
    bool held;
    // Distance from top of bar to mouse
    int mouse_dist;
};

struct Scrollbar* scrollbar_alloc(SDL_Rect rect);
void scrollbar_free(struct Scrollbar* self);

void scrollbar_render(struct Scrollbar* self, SDL_Renderer* rend, int mx, int my);

void scrollbar_update_units(struct Scrollbar* self, int total_units, int bar_len_units);

void scrollbar_follow_mouse(struct Scrollbar* self);

void scrollbar_scroll(struct Scrollbar* self, int y);

#endif

