#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

struct Node
{
    char* path;
    SDL_Texture* tex;

    struct Node** nodes;
    int node_num;

    bool opened;
};

struct Node* node_alloc(SDL_Renderer* rend, TTF_Font* font, char* path);
void node_free(struct Node* self);

// Renders node at point p
void node_render(struct Node* self, SDL_Renderer* rend, SDL_Point* p);

void node_toggle_opened(struct Node* self, SDL_Renderer* rend, TTF_Font* font);

struct Node* node_find_rect(struct Node* self, SDL_Point* start, int find_y);

static struct Node** node_read_dir_node(struct Node* self, SDL_Renderer* rend, TTF_Font* font, const char* path, int* count);

#endif

