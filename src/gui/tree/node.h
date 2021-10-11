#ifndef NODE_H
#define NODE_H

#include <SDL.h>
#include <SDL_ttf.h>

struct Node
{
    char* name;
    SDL_Texture* tex;

    struct Node** nodes;
    int node_num;
};

struct Node* node_alloc(SDL_Renderer* rend, TTF_Font* font, char* name);
void node_free(struct Node* self);

// Renders node at point p
void node_render(struct Node* self, SDL_Renderer* rend, SDL_Point* p);

void node_read_subnodes(struct Node* self, SDL_Renderer* rend, TTF_Font* font);

static struct Node** node_read_dir_node(struct Node* self, SDL_Renderer* rend, TTF_Font* font, const char* path, int* count);
static char** node_read_dir_type(struct Node* self, const char* path, unsigned char type, int* count);

#endif

