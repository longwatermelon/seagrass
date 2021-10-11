#ifndef TREE_H
#define TREE_H

#include "node.h"

struct Tree
{
    const char* path;
    SDL_Point pos;

    TTF_Font* font;
    SDL_Point char_dim;

    struct Node* root;
};

struct Tree* tree_alloc(SDL_Point pos, const char* path, SDL_Renderer* rend);
void tree_free(struct Tree* self);

void tree_render(struct Tree* self, SDL_Renderer* rend);

#endif

