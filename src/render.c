#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"
#include "render.h"

static void renderObjectAsRect(SDL_Renderer *ren, gameObject o, int r, int g, int b);
static void renderPlayer(SDL_Renderer *ren, gameObject player);
static void renderBullets(SDL_Renderer* ren, gameObject bullet[]);
static void renderEnemies(SDL_Renderer *ren, gameObject enemy[], SDL_Texture *texture[]);
static void renderText(SDL_Renderer *ren, TTF_Font *font, char text[], int x, int y, int r, int g, int b);

extern int gameState;

void render(SDL_Renderer *ren, TTF_Font *font, gameObject player, gameObject enemy[], SDL_Texture *enemyTexture[], gameObject bullet[])
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    renderBullets(ren, bullet);
    renderPlayer(ren, player);
    renderEnemies(ren, enemy, enemyTexture);

    if (gameState == 1)
    {
        renderText(ren, font, "Pause", 0, 0, 255, 255, 255);
    }
    else if (gameState == 2)
    {
        renderText(ren, font, "Game over", 0, 0, 255, 255, 255);
    }

    SDL_RenderPresent(ren);
}

static void renderObjectAsRect(SDL_Renderer *ren, gameObject o, int r, int g, int b)
{
    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_RenderFillRect(ren, &o.rect);
}

static void renderPlayer(SDL_Renderer *ren, gameObject player)
{
    renderObjectAsRect(ren, player, 0, 0, 255);
}

static void renderBullets(SDL_Renderer* ren, gameObject bullet[])
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

static void renderEnemies(SDL_Renderer *ren, gameObject enemy[], SDL_Texture *texture[])
{
    int i = 0;
    while (i < ENEMY_COUNT)
    {
        if (enemy[i].active)
        {
            //renderObjectAsRect(ren, enemy[i], enemy[i].color.r, enemy[i].color.g, enemy[i].color.b);
            SDL_RenderCopy(ren, texture[i], NULL, &enemy[i].rect);
        }
        i++;
    }
}

static void renderText(SDL_Renderer *ren, TTF_Font *font, char text[], int x, int y, int r, int g, int b)
{
    static char renderedText[15];

    static SDL_Surface* surface;
    static SDL_Texture* texture;

    static SDL_Rect dstRect;

    if (renderedText == text)
    {
        SDL_RenderCopy(ren, texture, NULL, &dstRect);
    }
    else
    {
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        SDL_Color color = {r, g, b};
        surface = TTF_RenderText_Solid(font, text, color);

        if (!surface)
        {
            printf("Failed to create text surface: %s\n", TTF_GetError());
        }

        texture = SDL_CreateTextureFromSurface(ren, surface);

        if (!texture) {
            printf("Failed to create text texture: %s\n", SDL_GetError());
        }

        // Get the dimensions of the texture
        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

        // Set the destination rectangle for rendering the texture
        dstRect.x = x;
        dstRect.y = y;
        dstRect.w = texW;
        dstRect.h = texH;

        SDL_RenderCopy(ren, texture, NULL, &dstRect);
    }
}