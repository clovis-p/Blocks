#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "main.h"
#include "init.h"
#include "collision.h"
#include "event.h"
#include "bullet.h"
#include "render.h"

int quit;
int shoot;
int reset;

int gameState; // 0: Normal in-game state, 1: Paused game state (not implemented yet), 2: Game over state

int main(int argc, char **argv)
{
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf("Error initializing SDL_image: %s\n", SDL_GetError());
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
    SDL_Texture *enemyTexture[ENEMY_COUNT];

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

    init(ren, &player, &enemy, &enemyTexture, &bullet);

    reset = 0;
    int remainingEnemies = ENEMY_COUNT;

    uint32_t previousTicks = 0;
    uint32_t currentTicks = 0;

    uint32_t resetTimerPreviousTicks = 0;

    gameState = 0; // 0: Normal in-game state, 1: Paused game state (not implemented yet), 2: Game over state

    int lol;

    while (!quit)
    {
        if (!remainingEnemies && gameState == 0)
            reset = 1;

        if (reset)
        {
            gameState = 2;
            resetTimerPreviousTicks = SDL_GetTicks();
            printf("reset %d %d\n", reset, gameState);
            reset = 0;
        }

        if (gameState == 0)
        {
            checkIfWithinBounds(&player, SET_COLLISION);

            checkEnemyCollisions(&enemy, player); // check collisions and update enemy pos
        }
        else if (gameState == 2)
        {
            if (currentTicks - resetTimerPreviousTicks < 2000)
            {
                printf("%d\n", currentTicks - resetTimerPreviousTicks);
                lol = 1;
            }
            else if (lol == 1)
            {
                bullet_i = 0;
                enemy_i = 0;
                printf("init\n\n");
                init(ren, &player, &enemy, &enemyTexture, &bullet);
                gameState = 0;
                lol = 0;
            }
        }

        /*  About the "lol" variable: I do not understand why this is needed, but apparently it is, otherwise the 2000ms
         *  delay does not happen. Removing "gameState = 0;" makes the delay work for some reason, but it is necessary
         *  to set it to zero to restart the game. */

        handleEvents(&player, &bullet, event);

        while (bullet_i < 50 && !reset) // apply damage to shot enemies and count remaining enemies
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

        render(ren, player, enemy, enemyTexture, bullet);

        currentTicks = SDL_GetTicks();

        if (currentTicks - previousTicks < 10)
            SDL_Delay(10 - (currentTicks - previousTicks));  // Maintain a constant frame rate

        previousTicks = SDL_GetTicks();
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);

    IMG_Quit();
    SDL_Quit();
    return 0;
}
