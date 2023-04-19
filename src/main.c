#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "init.h"
#include "collision.h"
#include "event.h"
#include "bullet.h"
#include "render.h"

int quit;
int shoot;
int reset;

int main(int argc, char **argv)
{
    return 0;
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    SDL_Event event;

    gameObject player;
    gameObject bullet[51];
    int bullet_i = 0;
    int enemy_i = 0;
    gameObject enemy[ENEMY_COUNT];

    init(&player, &enemy, &bullet);

    quit = 0;
    shoot = 0;

    win = SDL_CreateWindow("Blocks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION_X, RESOLUTION_Y, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    if (!win)
    {
        printf("Window init error: %s\n", SDL_GetError());
        return 2;
    }

    if (!ren)
    {
        printf("Renderer init error: %s\n", SDL_GetError());
        return 3;
    }

    reset = 0;
    int remainingEnemies = ENEMY_COUNT;

    uint32_t previousTicks = 0;
    uint32_t currentTicks = 0;

    while (!quit)
    {
        if (!remainingEnemies)
            reset = 1;

        if (reset)
        {
            SDL_Delay(2000);
            bullet_i = 0;
            enemy_i = 0;

            init(&player, &enemy, &bullet);

            reset = 0;
        }

        handleEvents(&player, &bullet, event);

        checkIfWithinBounds(&player, SET_COLLISION);

        checkEnemyCollisions(&enemy, player);

        while (bullet_i < 50 && !reset)
        {
            enemy_i = 0;
            remainingEnemies = 0;
            while (enemy_i < ENEMY_COUNT)
            {
                if (enemy[enemy_i].active)
                {
                    checkIfWithinBounds(&bullet[bullet_i], SET_INACTIVE);

                    if (bullet[bullet_i].active && overlap(bullet[bullet_i], enemy[enemy_i]))
                    {
                        enemy[enemy_i].rect.w -= 1;
                        enemy[enemy_i].rect.h -= 1;
                        if (enemy[enemy_i].rect.w < 10)
                        {
                            enemy[enemy_i].active = 0;
                        }
                    }
                    remainingEnemies++;
                }
                enemy_i++;
            }
            enemy_i = 0;

            bullet_i++;
        }
        bullet_i = 0;

        updateBulletsPos(&bullet);

        render(ren, player, enemy, bullet);

        currentTicks = SDL_GetTicks();

        if (currentTicks - previousTicks < 10)
            SDL_Delay(10 - (currentTicks - previousTicks));  // Maintain a constant frame rate

        previousTicks = SDL_GetTicks();
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);

    SDL_Quit();
    return 0;
}
