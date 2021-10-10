#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

struct Text
{
    SDL_Point pos;

    const char* contents;

    SDL_Texture** textures;
    int ntextures;

    TTF_Font* font;
    SDL_Point char_dim;
    SDL_Color color;
};

struct Text* text_alloc(SDL_Renderer* rend, SDL_Point pos, const char* contents, TTF_Font* font, SDL_Color color);
void text_free(struct Text* self);

void text_render(struct Text* self, SDL_Renderer* rend);

static SDL_Texture** text_split_str_into_textures(struct Text* self, SDL_Renderer* rend, int* count);
static SDL_Texture* text_render_text(struct Text* self, SDL_Renderer* rend, const char* text);

#endif

