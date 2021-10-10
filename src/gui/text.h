#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

struct Text
{
    SDL_Point pos;

    char** lines;
    SDL_Texture** textures;
    int nlines;

    TTF_Font* font;
    SDL_Point char_dim;
    SDL_Color color;
};

struct Text* text_alloc(SDL_Renderer* rend, SDL_Point pos, const char* contents, TTF_Font* font, SDL_Color color);
void text_free(struct Text* self);

void text_render(struct Text* self, SDL_Renderer* rend);

void text_redo_textures(struct Text* self, SDL_Renderer* rend);
void text_redo_texture(struct Text* self, SDL_Renderer* rend, int index, const char* text);
void text_insert_texture(struct Text* self, SDL_Renderer* rend, int index, const char* text);

// Split up regular string by \n into multiple textures
static SDL_Texture** text_split_str_into_textures(struct Text* self, SDL_Renderer* rend);

#endif

