#ifndef TREE_H
#define TREE_H

#include "node.h"

struct Tree
{
    const char* path;
    SDL_Point pos;

    struct Node** nodes;
    int node_num;

    TTF_Font* font;
};

struct Tree* tree_alloc(SDL_Point pos, const char* path, SDL_Renderer* rend);
void tree_free(struct Tree* self);

void tree_render(struct Tree* self, SDL_Renderer* rend);

struct Node** tree_read_dir_nodes(struct Tree* self, SDL_Renderer* rend, const char* path, int* count);
char** tree_read_dir_type(struct Tree* self, const char* path, unsigned char type, int* count);

#endif

