#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Button
{
    bool down;
    bool hover;

    SDL_Rect rect;
    SDL_Texture* tex;

    SDL_Color color;
};

struct Button* button_alloc(SDL_Rect rect, const char* text, SDL_Color col, SDL_Renderer* rend, TTF_Font* font);
void button_free(struct Button* self);

void button_render(struct Button* self, SDL_Renderer* rend);

void button_check_clicked(struct Button* self, int mx, int my);
void button_check_hover(struct Button* self, int mx, int my);

#endif

