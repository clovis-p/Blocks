#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"
#include "init.h"

extern int enemyCount;

extern int gameState;

int init(SDL_Window **win, SDL_Renderer **ren, TTF_Font **font)
{
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf("Error initializing SDL_image: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0)
    {
        printf("Error initializing SDL_ttf: %s\n", SDL_GetError());
        return -1;
    }

    *win = SDL_CreateWindow("Blocks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION_X, RESOLUTION_Y, SDL_WINDOW_SHOWN);

    if (!*win)
    {
        printf("Window init error: %s\n", SDL_GetError());
        return -1;
    }

    *ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);

    if (!*ren)
    {
        printf("Renderer init error: %s\n", SDL_GetError());
        return -1;
    }

    *font = TTF_OpenFont("../resources/font/3DIsometric-Bold.ttf", (RESOLUTION_Y / 10));

    if (!*font)
    {
        printf("Failed to open font: %s\n", SDL_GetError());
        return -1;
    }

    return 1;
}

void initGame(SDL_Renderer *ren, gameObject *player, gameObject *enemy, SDL_Texture **enemyTexture, gameObject *bullet)
{
    if (gameState == 2)
    {
        enemyCount++;
    }
    else if (gameState == 3)
    {
        enemyCount = 1;
    }
    int enemy_i = 0;
    int bullet_i = 0;

    char file[100];

    const int min_brightness = 100;

    player->color.r = 0;
    player->color.g = 0;
    player->color.b = 255;

    player->active = 720 / RESOLUTION_Y;

    player->speed = 3.0;

    player->fp.w = 40.0;
    player->fp.h = 40.0;
    player->rect.w = player->fp.w / 1280 * RESOLUTION_X_F;
    player->rect.h = player->fp.h / 720 * RESOLUTION_Y_F;
    player->fp.x = 1280 / 2 - player->fp.w / 2;
    player->fp.y = 720 / 2 - player->fp.h / 2;
    player->rect.x = player->fp.x;
    player->rect.y = player->fp.y;


    while (bullet_i < 50)
    {
        bullet[bullet_i].rect.x = 0;
        bullet[bullet_i].rect.y = 0;
        bullet[bullet_i].fp.w = 8.0;
        bullet[bullet_i].fp.h = 8.0;
        bullet[bullet_i].rect.w = bullet[bullet_i].fp.w / 1280.0 * RESOLUTION_X_F;
        bullet[bullet_i].rect.h = bullet[bullet_i].fp.w / 720.0 * RESOLUTION_Y_F;
        bullet[bullet_i].active = 0;
        bullet[bullet_i].direction = 0;
        bullet[bullet_i].speed = 8.0;
        bullet_i++;
    }

    while (enemy_i < enemyCount)
    {
        enemy[enemy_i].speed = 1.0;

        enemy[enemy_i].color.r = rand() % (256 - min_brightness) + min_brightness; // random number, min: min_brightness, max 255
        enemy[enemy_i].color.g = rand() % (256 - min_brightness) + min_brightness;
        enemy[enemy_i].color.b = rand() % (256 - min_brightness) + min_brightness;

        enemy[enemy_i].fp.w = 40.0;
        enemy[enemy_i].fp.h = 40.0;
        enemy[enemy_i].rect.w = enemy[enemy_i].fp.w / 1280 * RESOLUTION_X_F;
        enemy[enemy_i].rect.h = enemy[enemy_i].fp.h / 720 * RESOLUTION_Y_F;

        if (enemy_i == 0)
            enemy[enemy_i].fp.x = 0;
        else if (rand() % 2) // randomize enemy start points (flip a coin, top or bottom)
        {
            enemy[enemy_i].fp.x = 1280 / enemyCount * enemy_i;
            enemy[enemy_i].fp.y = 0;
        }
        else
        {
            enemy[enemy_i].fp.x = 1280 / enemyCount * enemy_i;
            enemy[enemy_i].fp.y = 720 - enemy[enemy_i].fp.h;
        }

        enemy[enemy_i].rect.x = enemy[enemy_i].fp.x / 1280 * RESOLUTION_X_F;
        enemy[enemy_i].rect.y = enemy[enemy_i].fp.y / 720 * RESOLUTION_Y_F;
        enemy[enemy_i].active = 1;

        sprintf(file, "../resources/enemy/%d.png", ((rand() % 9) + 1));
        enemyTexture[enemy_i] = loadImageAsTexture(ren, file);

        enemy_i++;
    }
}

SDL_Texture *loadImageAsTexture(SDL_Renderer *ren, char file[])
{
    SDL_Surface *imageSurface = IMG_Load(file);
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(ren, imageSurface);

    return imageTexture;
}
