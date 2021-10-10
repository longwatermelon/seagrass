#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "text.h"

struct Textbox
{
    SDL_Rect rect;
    struct Text* text;
    SDL_Color bg_color;

    // Measured as position in characters
    SDL_Point cursor_pos;
};

struct Textbox* textbox_alloc(SDL_Rect rect, SDL_Renderer* rend, TTF_Font* font, SDL_Color color);
void textbox_free(struct Textbox* self);

void textbox_render(struct Textbox* self, SDL_Renderer* rend);

void textbox_move_cursor(struct Textbox* self, int x, int y);

// Jump to end of line if cursor is in an illegal position
void textbox_cond_jump_to_eol(struct Textbox* self);

// Converts character coordinates (row, col) to pixel coordinates
static SDL_Point textbox_char_to_pix_pos(struct Textbox* self, SDL_Point pos);

#endif

