#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "text.h"
#include <stdbool.h>

struct Textbox
{
    SDL_Rect rect;
    struct Text* text;
    SDL_Color bg_color;

    // Measured in characters
    SDL_Point cursor_pos;

    // Measured in characters
    SDL_Point view_pos;

    bool highlighting;
    // Measured in chars
    SDL_Point highlight_begin, highlight_end;
};

struct Textbox* textbox_alloc(SDL_Rect rect, SDL_Renderer* rend, TTF_Font* font, SDL_Color color);
void textbox_free(struct Textbox* self);

void textbox_render(struct Textbox* self, SDL_Renderer* rend, bool render_cursor);
// Render highlighted sections
void textbox_render_highlight(struct Textbox* self, SDL_Renderer* rend);
// Highlight a line from index begin to end at index.
void textbox_render_highlight_line(struct Textbox* self, SDL_Renderer* rend, int index, int begin, int end);

void textbox_move_cursor(struct Textbox* self, int x, int y);
// Jump to end of line if cursor is in an illegal position
void textbox_cond_jump_to_eol(struct Textbox* self);
// Move cursor to mouse position
void textbox_cursor_follow_mouse(struct Textbox* self, int mx, int my);

// Adds character where cursor currently is
void textbox_add_char(struct Textbox* self, SDL_Renderer* rend, char c);
// Add new line
void textbox_add_nl(struct Textbox* self, SDL_Renderer* rend);
// Backspace on the current character
void textbox_del_char(struct Textbox* self, SDL_Renderer* rend);
// Remove an entire line
void textbox_del_nl(struct Textbox* self, SDL_Renderer* rend);

// Move the view by (x, y) characters
void textbox_move_view(struct Textbox* self, int x, int y);
// Move view_pos to cursor if necessary
void textbox_move_view_cursor(struct Textbox* self);

// Deletes all currently highlighted lines.
void textbox_del_highlighted(struct Textbox* self, SDL_Renderer* rend);
// Deletes line at index from begin to end.
void textbox_del_highlighted_line(struct Textbox* self, SDL_Renderer* rend, int index, int begin, int end);

// Converts character coordinates (row, col) to pixel coordinates
static SDL_Point textbox_char_to_pix_pos(struct Textbox* self, SDL_Point pos);

#endif

