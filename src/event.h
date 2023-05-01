#ifndef EVENT_H
#define EVENT_H

void handleEvents(gameObject *player, gameObject *bullet, SDL_Event event);

extern int quit;
extern int shoot;
extern int gameState;

#endif // EVENT_H
