#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>
struct Prog;

void events_base(struct Prog* p, SDL_Event* evt);

void events_keydown(struct Prog* p, SDL_Event* evt);
void events_keyup(struct Prog* p, SDL_Event* evt);

void events_mouse(struct Prog* p, SDL_Event* evt);
void events_mouse_left(struct Prog* p, SDL_Event* evt);
void events_mouse_release(struct Prog* p, SDL_Event* evt);

void events_mousewheel(struct Prog* p, SDL_Event* evt);

#endif

