#ifndef PROG_H
#define PROG_H

#include "gui/scrollbar.h"
#include "gui/tree.h"
#include <stdbool.h>
#include <dirent.h>
#include <SDL.h>
#include <SDL_ttf.h>

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
};

struct Prog* prog_alloc();
void prog_free(struct Prog* self);

void prog_mainloop(struct Prog* self);
void prog_render(struct Prog* self);

void prog_mainloop_textbox(struct Prog* self);
void prog_mainloop_scrollbar(struct Prog* self);

void prog_open_file(struct Prog* self, const char* fp);
void prog_save_file(struct Prog* self);

#endif

