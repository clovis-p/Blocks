#ifndef EVENT_H
#define EVENT_H

void handleEvents(gameObject *player, gameObject *bullet, SDL_Event event);
int fullscreenToggleRequested();

extern int quit;
extern int shoot;

#endif // EVENT_H
