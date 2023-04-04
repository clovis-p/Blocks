#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"

void renderObjectAsRect(SDL_Renderer *ren, gameObject o, int r, int g, int b);
void renderPlayer(SDL_Renderer *ren, gameObject player);
void renderBullets(SDL_Renderer* ren, gameObject bullet[]);
void renderEnemies(SDL_Renderer *ren, gameObject enemy[]);

void render(SDL_Renderer *ren, gameObject player, gameObject enemy[], gameObject bullet[])
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    renderBullets(ren, bullet);
    renderPlayer(ren, player);
    renderEnemies(ren, enemy);

    SDL_RenderPresent(ren);
}

void renderObjectAsRect(SDL_Renderer *ren, gameObject o, int r, int g, int b)
{
    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_RenderFillRect(ren, &o.rect);
}

void renderPlayer(SDL_Renderer *ren, gameObject player)
{
    renderObjectAsRect(ren, player, 0, 0, 255);
}

void renderBullets(SDL_Renderer* ren, gameObject bullet[])
{
    int i = 0;
    while (i < 50)
    {
        if (bullet[i].active)
        {
            renderObjectAsRect(ren, bullet[i], 255, 255, 255);
        }
        i++;
    }
}

void renderEnemies(SDL_Renderer *ren, gameObject enemy[])
{
    int i = 0;
    while (i < ENEMY_COUNT)
    {
        if (enemy[i].active)
        {
            renderObjectAsRect(ren, enemy[i], enemy[i].color.r, enemy[i].color.g, enemy[i].color.b);
        }
        i++;
    }
}
