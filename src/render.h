#ifndef RENDER_H
#define RENDER_H

void render(SDL_Window *win, SDL_Renderer *ren, TTF_Font **font, gameObject player, gameObject enemy[], SDL_Texture *enemyTexture[], gameObject bullet[]);

#endif // RENDER_H
