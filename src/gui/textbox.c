#include "textbox.h"


struct Textbox* textbox_alloc(SDL_Rect rect, SDL_Renderer* rend, TTF_Font* font, SDL_Color color)
{
    struct Textbox* self = malloc(sizeof(struct Textbox));
    self->rect = rect;

    self->text = text_alloc(rend, (SDL_Point){ rect.x, rect.y }, "", font, (SDL_Color){ 255, 255, 255 });
    self->bg_color = color;

    self->cursor_pos = (SDL_Point){ .x = 0, .y = 0 };

    return self;
}


void textbox_free(struct Textbox* self)
{
    text_free(self->text);
    free(self);
}


void textbox_render(struct Textbox* self, SDL_Renderer* rend)
{
    SDL_SetRenderDrawColor(rend, self->bg_color.r, self->bg_color.g, self->bg_color.b, 255);
    SDL_RenderFillRect(rend, &self->rect);

    text_render(self->text, rend);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_Point pos = textbox_char_to_pix_pos(self, self->cursor_pos);

    SDL_Rect cursor = { .x = pos.x, .y = pos.y, .w = 2, .h = self->text->char_dim.y };
    SDL_RenderFillRect(rend, &cursor);
}


void textbox_move_cursor(struct Textbox* self, int x, int y)
{
    if (self->cursor_pos.x + x >= 0 &&
            self->cursor_pos.x + x <= strlen(self->text->lines[self->cursor_pos.y]))
        self->cursor_pos.x += x;

    if (self->cursor_pos.y + y >= 0 &&
            self->cursor_pos.y + y < self->text->nlines)
    {
        self->cursor_pos.y += y;
        textbox_cond_jump_to_eol(self);
    }
}


void textbox_cond_jump_to_eol(struct Textbox* self)
{
    if (self->cursor_pos.x > strlen(self->text->lines[self->cursor_pos.y]))
        self->cursor_pos.x = strlen(self->text->lines[self->cursor_pos.y]);
}


void textbox_add_char(struct Textbox* self, SDL_Renderer* rend, char c)
{
    char* curr_line = self->text->lines[self->cursor_pos.y];
    int len = strlen(curr_line) + 1;

    curr_line = realloc(curr_line, sizeof(char) * (len + 1));
    curr_line[len] = '\0';

    memcpy(&curr_line[self->cursor_pos.x + 1],
           &curr_line[self->cursor_pos.x],
           sizeof(char) * (strlen(curr_line) - self->cursor_pos.x));

    curr_line[self->cursor_pos.x] = c;

    text_redo_texture(self->text, rend, self->cursor_pos.y, curr_line);
    textbox_move_cursor(self, 1, 0);
}


static SDL_Point textbox_char_to_pix_pos(struct Textbox* self, SDL_Point pos)
{
    return (SDL_Point){
        .x = self->rect.x + (pos.x * self->text->char_dim.x),
        .y = self->rect.y + (pos.y * self->text->char_dim.y)
    };
}

