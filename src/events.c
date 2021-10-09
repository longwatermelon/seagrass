#include "events.h"
#include "prog.h"


void events_base(struct Prog* p, SDL_Event* evt)
{
    while (SDL_PollEvent(evt))
    {
        switch (evt->type)
        {
        case SDL_QUIT:
            p->running = false;
            break;
        }
    }
}

