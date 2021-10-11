#include "tree.h"
#include "../utils.h"
#include <dirent.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


struct Tree* tree_alloc(SDL_Point pos, const char* path, SDL_Renderer* rend)
{
    struct Tree* self = malloc(sizeof(struct Tree));
    self->path = path;
    self->pos = pos;
    self->path = path;

    self->font = TTF_OpenFont("res/font.ttf", 14);
    TTF_SizeText(self->font, " ", &self->char_dim.x, &self->char_dim.y);

    char* abspath = realpath(path, 0);
    self->root = node_alloc(rend, self->font, abspath, DT_DIR);

    node_toggle_opened(self->root, rend, self->font);

    self->ntextures = 2;
    self->textures[TEX_FOLDER] = IMG_LoadTexture(rend, "res/folder.png");
    self->textures[TEX_FILE] = IMG_LoadTexture(rend, "res/file.png");

    return self;
}


void tree_free(struct Tree* self)
{
    node_free(self->root);
    TTF_CloseFont(self->font);

    for (int i = 0; i < self->ntextures; ++i)
        SDL_DestroyTexture(self->textures[i]);

    free(self);
}


void tree_render(struct Tree* self, SDL_Renderer* rend)
{
    // SDL_Point* passed in will be modified so copy
    SDL_Point start = (SDL_Point){ .x = self->pos.x, .y = self->pos.y - self->char_dim.y };
    node_render(self->root, rend, &start, self->textures);
}



struct Node* tree_clicked(struct Tree* self, int mx, int my)
{
    SDL_Point start = (SDL_Point){ .x = self->pos.x, .y = self->pos.y - self->char_dim.y };
    return node_find_rect(self->root, &start, my);
}

