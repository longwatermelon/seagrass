#include "node.h"
#include "../utils.h"


struct Node* node_alloc(SDL_Renderer* rend, TTF_Font* font, char* name)
{
    struct Node* self = malloc(sizeof(struct Node));
    self->name = name;
    self->tex = utils_render_text(rend, name, font, (SDL_Color){ 255, 255, 255 });

    self->nodes = malloc(0);
    self->node_num = 0;

    return self;
}


void node_free(struct Node* self)
{
    for (int i = 0; i < self->node_num; ++i)
        node_free(self->nodes[i]);

    free(self->nodes);

    SDL_DestroyTexture(self->tex);
    free(self->name);

    free(self);
}


void node_render(struct Node* self, SDL_Renderer* rend, SDL_Point* p)
{
    SDL_Rect rect = { .x = p->x, .y = p->y };
    SDL_QueryTexture(self->tex, 0, 0, &rect.w, &rect.h);

    SDL_RenderCopy(rend, self->tex, 0, &rect);
    p->y += rect.h;

    for (int i = 0; i < self->node_num; ++i)
    {
        node_render(self->nodes[i], rend, p);
        p->y += rect.h;
    }
}

