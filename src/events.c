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
        case SDL_TEXTINPUT:
            if (p->selected_textbox)
                textbox_add_char(p->selected_textbox, p->rend, evt->text.text[0]);
            break;
        case SDL_KEYDOWN:
            events_keydown(p, evt);
            break;
        }
    }
}


void events_keydown(struct Prog* p, SDL_Event* evt)
{
    if (p->selected_textbox)
    {
        switch (evt->key.keysym.sym)
        {
        case SDLK_RIGHT:
            textbox_move_cursor(p->selected_textbox, 1, 0);
            break;
        case SDLK_LEFT:
            textbox_move_cursor(p->selected_textbox, -1, 0);
            break;
        case SDLK_UP:
            textbox_move_cursor(p->selected_textbox, 0, -1);
            break;
        case SDLK_DOWN:
            textbox_move_cursor(p->selected_textbox, 0, 1);
            break;
        }

        switch (evt->key.keysym.scancode)
        {
        case SDL_SCANCODE_RETURN:
            textbox_add_nl(p->selected_textbox, p->rend);
            break;
        case SDL_SCANCODE_BACKSPACE:
            textbox_del_char(p->selected_textbox, p->rend);
            break;
        default:
            break;
        }
    }
}

