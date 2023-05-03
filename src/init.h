#ifndef INIT_H
#define INIT_H

int init(SDL_Window **win, SDL_Renderer **ren);
void initGame(SDL_Renderer *ren, gameObject *player, gameObject *enemy, SDL_Texture **enemyTexture, gameObject *bullet);
SDL_Texture *loadImageAsTexture(SDL_Renderer *ren, char file[]);

#endif // INIT_H
