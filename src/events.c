#include "events.h"
#include "prog.h"
#include "gui/textbox.h"


void events_base(struct Prog* p, SDL_Event* evt)
{
    while (SDL_PollEvent(evt))
    {
        switch (evt->type)
        {
        case SDL_QUIT:
            p->running = false;
            break;
        case SDL_KEYDOWN:
            events_keydown(p, evt);
        }
    }
}


void events_keydown(struct Prog* p, SDL_Event* evt)
{
    switch (evt->key.keysym.sym)
    {
    case SDLK_RIGHT:
        if (p->selected_textbox)
            textbox_move_cursor(p->selected_textbox, 1, 0);
        break;
    case SDLK_LEFT:
        if (p->selected_textbox)
            textbox_move_cursor(p->selected_textbox, -1, 0);
        break;
    case SDLK_UP:
        if (p->selected_textbox)
            textbox_move_cursor(p->selected_textbox, 0, -1);
        break;
    case SDLK_DOWN:
        if (p->selected_textbox)
            textbox_move_cursor(p->selected_textbox, 0, 1);
        break;
    }
}

