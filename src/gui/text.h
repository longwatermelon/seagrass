#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

// view_pos measured in chars; pass in (0, 0) if not applicable
// enclosing_rect: pass in 0 if not applicable
void text_render(struct Text* self, SDL_Renderer* rend, SDL_Point view_pos, SDL_Rect* enclosing_rect);

void text_redo_textures(struct Text* self, SDL_Renderer* rend);

// Redo and insert only copy the contents of text, not the pointer itself. Free the string passed in if it's dynamically allocated.
void text_redo_texture(struct Text* self, SDL_Renderer* rend, int index, const char* text);
void text_insert_texture(struct Text* self, SDL_Renderer* rend, int index, const char* text);
void text_remove_texture(struct Text* self, SDL_Renderer* rend, int index);

// Get all text as one large string
char* text_as_str(struct Text* self);

// Split up regular string by \n into multiple textures
SDL_Texture** text_split_str_into_textures(struct Text* self, SDL_Renderer* rend);

#endif

