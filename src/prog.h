#ifndef PROG_H
#define PROG_H

#include "gui/scrollbar.h"
#include "gui/tree.h"
#include "gui/button.h"
#include "gui/text.h"
#include <stdbool.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Prog
{
    bool running;

    SDL_Window* window;
    SDL_Renderer* rend;

    TTF_Font* font;

    char opened_file[PATH_MAX];

    struct Textbox* main_textbox;
    struct Scrollbar* main_scrollbar;
    struct Tree* file_tree;

    struct Textbox* selected_textbox;

    // Binary file opened warning
    bool binary_show_warning;
    struct Text* binary_warning_text;
    struct Button* binary_confirm_btn;
};

struct Prog* prog_alloc();
void prog_free(struct Prog* self);

void prog_mainloop(struct Prog* self);
void prog_render(struct Prog* self);

void prog_mainloop_textbox(struct Prog* self);
void prog_mainloop_scrollbar(struct Prog* self);
void prog_mainloop_binary_warning(struct Prog* self);

void prog_render_binary_warning(struct Prog* self);

void prog_open_file(struct Prog* self, const char* fp);
void prog_save_file(struct Prog* self);

#endif

