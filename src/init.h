#ifndef INIT_H
#define INIT_H

int init(SDL_Window **win, Uint32 flags, SDL_Renderer **ren, TTF_Font **font);
void initGame(SDL_Renderer *ren, gameObject *player, gameObject *enemy, SDL_Texture **enemyTexture, gameObject *bullet);
void applyNewResolution(SDL_Window *win, SDL_Renderer *ren, TTF_Font **font, gameObject *player, gameObject *enemy, gameObject *bullet);
void scaleObjectToWindowRes(SDL_Rect *rect, fpRect *fp);
SDL_Texture *loadImageAsTexture(SDL_Renderer *ren, char file[]);

#endif // INIT_H
