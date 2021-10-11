#ifndef TREE_H
#define TREE_H

#include "node.h"

enum
{
    TEX_FOLDER,
    TEX_FILE
};

struct Tree
{
    const char* path;
    SDL_Point pos;

    TTF_Font* font;
    SDL_Point char_dim;

    struct Node* root;

    SDL_Texture* textures[2];
    int ntextures;
};

struct Tree* tree_alloc(SDL_Point pos, const char* path, SDL_Renderer* rend);
void tree_free(struct Tree* self);

void tree_render(struct Tree* self, SDL_Renderer* rend);

struct Node* tree_clicked(struct Tree* self, int mx, int my);

#endif

