#ifndef UTILS_GUI_H
#define UTILS_GUI_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* utils_render_text(SDL_Renderer* rend, const char* text, TTF_Font* font, SDL_Color color);

char* utils_substr(const char* str, int start, int end);
char** utils_split_str(const char* str, char delim, int* count);

bool utils_p_in_rect(SDL_Point p, SDL_Rect rect);

void utils_sort_alphabetically(char** arr, int len);

char* utils_find_resource(const char* name);

#endif

