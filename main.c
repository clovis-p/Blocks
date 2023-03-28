#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "init.h"
#include "collision.h"

#define LEFT_COL 0
#define DOWN_COL 1
#define RIGHT_COL 2
#define UP_COL 3
#define OTHER_COL 4

int main(int argc, char **argv)
{
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
    int shoot = 0;
    int bullet_i = 0;
    int bullet_i2 = 0;
    int enemy_i = 0;
    int colEnemy = 0;
    int colDetected[5] = {0, 0, 0, 0, 0}; // 0 = left, 1 = down, 2 = right, 3 = up
    gameObject enemy[ENEMY_COUNT];

    init(&player, &enemy, &bullet);

    int speed = 3;

    int quit = 0;

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

    const Uint8* keyStates;

    int reset = 0;
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
            colEnemy = 0;

            colDetected[LEFT_COL] = 0;
            colDetected[DOWN_COL] = 0;
            colDetected[RIGHT_COL] = 0;
            colDetected[UP_COL] = 0;
            colDetected[OTHER_COL] = 0;

            init(&player, &enemy, &bullet);

            reset = 0;
        }

        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
        {
            quit = 1;
        }

        keyStates = SDL_GetKeyboardState(NULL);

        if (keyStates[SDL_SCANCODE_LSHIFT])
            speed = 6;
        else
            speed = 3;

        if (keyStates[SDL_SCANCODE_W] && player.rect.y > 0) // up
        {
            player.rect.y -= speed;
            player.direction = 3; // 0 = undefined, 1 = left, 2 = left/up, 3 = up, 4 = up/right, 5 = right, 6 = right/down, 7 = down, 8 = down/left
        }
        if (keyStates[SDL_SCANCODE_A] && player.rect.x > 0)
        {
            player.rect.x -= speed;
            player.direction = 1;
        }
        if (keyStates[SDL_SCANCODE_W] && keyStates[SDL_SCANCODE_A]) // up/left
        {
            player.direction = 2;
        }
        if (keyStates[SDL_SCANCODE_S] && player.rect.y + player.rect.h < RESOLUTION_Y)
        {
            player.rect.y += speed;
            player.direction = 7;
        }
        if (keyStates[SDL_SCANCODE_A] && keyStates[SDL_SCANCODE_S]) // left/down
        {
            player.direction = 8;
        }
        if (keyStates[SDL_SCANCODE_D] && player.rect.x + player.rect.w < RESOLUTION_X)
        {
            player.rect.x += speed;
            player.direction = 5;
        }
        if (keyStates[SDL_SCANCODE_S] && keyStates[SDL_SCANCODE_D]) // down/right
        {
            player.direction = 6;
        }
        if (keyStates[SDL_SCANCODE_D] && keyStates[SDL_SCANCODE_W]) // right/up
        {
            player.direction = 4;
        }

        if (keyStates[SDL_SCANCODE_SPACE] && !shoot) // shoot bullet
        {
            shoot = 1;
            bullet[bullet_i2].direction = player.direction;
            bullet[bullet_i2].active = 1;
            bullet[bullet_i2].rect.x = player.rect.x + player.rect.w / 2 - bullet[bullet_i2].rect.w / 2;
            bullet[bullet_i2].rect.y = player.rect.y + player.rect.h / 2 - bullet[bullet_i2].rect.h / 2;

            if (bullet_i2 < 49)
                bullet_i2++;
            else
                bullet_i2 = 0;
        }
        if (!keyStates[SDL_SCANCODE_SPACE])
        {
            shoot = 0;
        }

        enemy_i = 0;
        colDetected[LEFT_COL] = 0;
        colDetected[DOWN_COL] = 0;
        colDetected[RIGHT_COL] = 0;
        colDetected[UP_COL] = 0;
        colDetected[OTHER_COL] = 0;
        while (enemy_i < ENEMY_COUNT)
        {
            colEnemy = 0;
            colDetected[LEFT_COL] = 0;
            while (colEnemy < ENEMY_COUNT)
            {
                if (checkCollision(enemy[enemy_i], enemy[colEnemy]) &&
                    enemy[enemy_i].rect.x >= enemy[colEnemy].rect.x + 0.9 * enemy[colEnemy].rect.w &&
                    colEnemy != enemy_i)
                {
                    colDetected[LEFT_COL] = 1;
                }
                colEnemy++;
            }
            if (enemy[enemy_i].rect.x + enemy[enemy_i].rect.w / 2 > player.rect.x + player.rect.w / 2 && !colDetected[LEFT_COL] && enemy[enemy_i].active) // move left
                enemy[enemy_i].rect.x -= enemy[enemy_i].speed + rand() % 2;


            colEnemy = 0;
            colDetected[RIGHT_COL] = 0;
            while (colEnemy < ENEMY_COUNT)
            {
                if (checkCollision(enemy[enemy_i], enemy[colEnemy])&&
                    enemy[enemy_i].rect.x + enemy[enemy_i].rect.w <= enemy[colEnemy].rect.x + 0.1 * enemy[colEnemy].rect.w &&
                    colEnemy != enemy_i)
                {
                    colDetected[RIGHT_COL] = 1;
                }

                colEnemy++;
            }
            if (enemy[enemy_i].rect.x + enemy[enemy_i].rect.w / 2 < player.rect.x + player.rect.w / 2 && !colDetected[RIGHT_COL] && enemy[enemy_i].active) // move right
                enemy[enemy_i].rect.x += enemy[enemy_i].speed + rand() % 2;


            colEnemy = 0;
            colDetected[UP_COL] = 0;
            while (colEnemy < ENEMY_COUNT)
            {
                if (checkCollision(enemy[enemy_i], enemy[colEnemy]) &&
                    enemy[enemy_i].rect.y - (enemy[colEnemy].rect.y + enemy[colEnemy].rect.h) < enemy[enemy_i].speed && ///// ligne magique
                    enemy[enemy_i].rect.y >= enemy[colEnemy].rect.y + 0.9 * enemy[colEnemy].rect.h &&
                    colEnemy != enemy_i)
                {
                    colDetected[UP_COL] = 1;
                }

                colEnemy++;
            }
            if (enemy[enemy_i].rect.y + enemy[enemy_i].rect.h / 2 > player.rect.y + player.rect.h / 2 && !colDetected[UP_COL] && enemy[enemy_i].active) // move up
                enemy[enemy_i].rect.y -= enemy[enemy_i].speed + rand() % 2; // + 0 ou + 1

            colEnemy = 0;
            colDetected[DOWN_COL] = 0;
            while (colEnemy < ENEMY_COUNT)
            {
                if (checkCollision(enemy[enemy_i], enemy[colEnemy]) &&
                    enemy[enemy_i].rect.y + enemy[enemy_i].rect.h <= enemy[colEnemy].rect.y + 0.1 * enemy[colEnemy].rect.h &&
                    colEnemy != enemy_i)
                {
                    colDetected[DOWN_COL] = 1;
                }

                colEnemy++;
            }
            if (enemy[enemy_i].rect.y + enemy[enemy_i].rect.h / 2 < player.rect.y + player.rect.h / 2 && !colDetected[DOWN_COL] && enemy[enemy_i].active) // move down
                enemy[enemy_i].rect.y += enemy[enemy_i].speed + rand() % 2;

            colEnemy = 0;

            if (checkCollision(enemy[enemy_i], player))
            {
                reset = 1;
            }

            enemy_i++;
        }
        enemy_i = 0;

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

        while (bullet_i < 50 && !reset)
        {
            enemy_i = 0;
            remainingEnemies = 0;
            while (enemy_i < ENEMY_COUNT)
            {
                if (enemy[enemy_i].active)
                {
                    if (bullet[bullet_i].rect.x > RESOLUTION_X ||
                        bullet[bullet_i].rect.x + bullet[bullet_i].rect.w < 0 ||
                        bullet[bullet_i].rect.y + bullet[bullet_i].rect.h > RESOLUTION_Y ||
                        bullet[bullet_i].rect.y < 0)
                    {
                        bullet[bullet_i].active = 0;
                    }

                    if (bullet[bullet_i].active &&
                        bullet[bullet_i].rect.x + bullet[bullet_i].rect.w - 1 >= enemy[enemy_i].rect.x &&
                        bullet[bullet_i].rect.x <= enemy[enemy_i].rect.x + enemy[enemy_i].rect.w - 1 &&
                        bullet[bullet_i].rect.y + bullet[bullet_i].rect.h - 1 >= enemy[enemy_i].rect.y &&
                        bullet[bullet_i].rect.y <= enemy[enemy_i].rect.y + enemy[enemy_i].rect.h - 1)
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

            if (bullet[bullet_i].active)
            {   // 0 = undefined, 1 = left, 2 = left/up, 3 = up, 4 = up/right, 5 = right, 6 = right/down, 7 = down, 8 = down/left
                if (bullet[bullet_i].direction == 1 || bullet[bullet_i].direction == 0)
                {
                    bullet[bullet_i].rect.x -= bullet[bullet_i].speed;
                }
                if (bullet[bullet_i].direction == 2)
                {
                    bullet[bullet_i].rect.x -= bullet[bullet_i].speed;
                    bullet[bullet_i].rect.y -= bullet[bullet_i].speed;
                }
                if (bullet[bullet_i].direction == 3)
                {
                    bullet[bullet_i].rect.y -= bullet[bullet_i].speed;
                }
                if (bullet[bullet_i].direction == 4)
                {
                    bullet[bullet_i].rect.y -= bullet[bullet_i].speed;
                    bullet[bullet_i].rect.x += bullet[bullet_i].speed;
                }
                if (bullet[bullet_i].direction == 5)
                {
                    bullet[bullet_i].rect.x += bullet[bullet_i].speed;
                }
                if (bullet[bullet_i].direction == 6)
                {
                    bullet[bullet_i].rect.x += bullet[bullet_i].speed;
                    bullet[bullet_i].rect.y += bullet[bullet_i].speed;
                }
                if (bullet[bullet_i].direction == 7)
                {
                    bullet[bullet_i].rect.y += bullet[bullet_i].speed;
                }
                if (bullet[bullet_i].direction == 8)
                {
                    bullet[bullet_i].rect.y += bullet[bullet_i].speed;
                    bullet[bullet_i].rect.x -= bullet[bullet_i].speed;
                }
                SDL_RenderFillRect(ren, &bullet[bullet_i].rect);
            }
            bullet_i++;
        }
        bullet_i = 0;

        SDL_SetRenderDrawColor(ren, player.color.r, player.color.g, player.color.b, 255);
        SDL_RenderFillRect(ren, &player.rect);

        enemy_i = 0;
        while (enemy_i < ENEMY_COUNT)
        {
            if (enemy[enemy_i].active)
            {
                SDL_SetRenderDrawColor(ren, enemy[enemy_i].color.r, enemy[enemy_i].color.g, enemy[enemy_i].color.b, 255);
                SDL_RenderFillRect(ren, &enemy[enemy_i].rect);
            }
            enemy_i++;
        }
        enemy_i = 0;

        SDL_RenderPresent(ren);

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
