#include "button.h"
#include "utils.h"


struct Button* button_alloc(SDL_Rect rect, const char* text, SDL_Color col, SDL_Renderer* rend, TTF_Font* font)
{
    struct Button* self = malloc(sizeof(struct Button));
    self->down = false;
    self->hover = false;

    self->rect = rect;
    self->color = col;
    
    self->tex = utils_render_text(rend, text, font, (SDL_Color){ 255, 255, 255 });

    return self;
}


void button_free(struct Button* self)
{
    SDL_DestroyTexture(self->tex);
    free(self);
}


void button_render(struct Button* self, SDL_Renderer* rend)
{
    int shade = (self->down ? -30 : (self->hover ? 30 : 0));

    SDL_SetRenderDrawColor(rend, self->color.r + shade, self->color.g + shade, self->color.b + shade, 255);
    SDL_RenderFillRect(rend, &self->rect);

    SDL_Rect text_rect;
    SDL_QueryTexture(self->tex, 0, 0, &text_rect.w, &text_rect.h);

    text_rect.x = self->rect.x + self->rect.w / 2 - text_rect.w / 2;
    text_rect.y = self->rect.y + self->rect.h / 2 - text_rect.h / 2;

    SDL_RenderCopy(rend, self->tex, 0, &text_rect);
}


void button_check_clicked(struct Button* self, int mx, int my)
{
    if (utils_p_in_rect((SDL_Point){ mx, my }, self->rect))
        self->down = true;
}


void button_check_hover(struct Button* self, int mx, int my)
{
    self->hover = utils_p_in_rect((SDL_Point){ mx, my }, self->rect);
}

