#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_ttf.h>

SDL_Texture* utils_render_text(SDL_Renderer* rend, const char* text, TTF_Font* font, SDL_Color color);

char* utils_substr(const char* str, int start, int end);
char** utils_split_str(const char* str, char delim, int* count);

#endif

