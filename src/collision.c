#include <stdlib.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "collision.h"

extern int enemyCount;

int overlap(gameObject o1, gameObject o2)
{
    int o1top = o1.rect.y;
    int o1left = o1.rect.x;
    int o1bottom = o1.rect.y + o1.rect.h;
    int o1right = o1.rect.x + o1.rect.w;

    int o2top = o2.rect.y;
    int o2left = o2.rect.x;
    int o2bottom = o2.rect.y + o2.rect.h;
    int o2right = o2.rect.x + o2.rect.w;

    if (o1.active &&
        o2.active &&
        o1right >= o2left &&
        o1left <= o2right &&
        o1bottom >= o2top &&
        o1top <= o2bottom)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void checkIfWithinBounds(gameObject *o, int flag)
{
    if (o->rect.x <= 0)
    {
        if (flag == SET_COLLISION)
            o->collision.left = 1;
        if (flag == SET_INACTIVE)
            o->active = 0;
    }
    else
    {
        if (flag == SET_COLLISION)
            o->collision.left = 0;
    }
    if (o->rect.x + o->rect.w >= RESOLUTION_X)
    {
        if (flag == SET_COLLISION)
            o->collision.right = 1;
        if (flag == SET_INACTIVE)
            o->active = 0;
    }
    else
    {
        if (flag == SET_COLLISION)
            o->collision.right = 0;
    }
    if (o->rect.y <= 0)
    {
        o->collision.up = 1;
        if (flag == SET_INACTIVE)
            o->active = 0;
    }
    else
    {
        if (flag == SET_COLLISION)
            o->collision.up = 0;
    }
    if (o->rect.y + o->rect.h >= RESOLUTION_Y)
    {
        o->collision.down = 1;
        if (flag == SET_INACTIVE)
            o->active = 0;
    }
    else
    {
        if (flag == SET_COLLISION)
            o->collision.down = 0;
    }
}

void checkEnemyCollisions(gameObject *enemy, gameObject player)
{
    int enemy_i = 0;
    int colEnemy = 0;
    while (enemy_i < enemyCount)
    {
        colEnemy = 0;
        enemy[enemy_i].collision.left = 0;
        while (colEnemy < enemyCount)
        {
            if (overlap(enemy[enemy_i], enemy[colEnemy]) &&
                enemy[enemy_i].rect.x >= enemy[colEnemy].rect.x + 0.9 * enemy[colEnemy].rect.w &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.left = 1;
            }
            colEnemy++;
        }
        if (enemy[enemy_i].rect.x + enemy[enemy_i].rect.w / 2 > player.rect.x + player.rect.w / 2 && !enemy[enemy_i].collision.left && enemy[enemy_i].active) // move left
            enemy[enemy_i].rect.x -= enemy[enemy_i].speed + rand() % 2;


        colEnemy = 0;
        enemy[enemy_i].collision.right = 0;
        while (colEnemy < enemyCount)
        {
            if (overlap(enemy[enemy_i], enemy[colEnemy])&&
                enemy[enemy_i].rect.x + enemy[enemy_i].rect.w <= enemy[colEnemy].rect.x + 0.1 * enemy[colEnemy].rect.w &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.right = 1;
            }

            colEnemy++;
        }
        if (enemy[enemy_i].rect.x + enemy[enemy_i].rect.w / 2 < player.rect.x + player.rect.w / 2 && !enemy[enemy_i].collision.right && enemy[enemy_i].active) // move right
            enemy[enemy_i].rect.x += enemy[enemy_i].speed + rand() % 2;


        colEnemy = 0;
        enemy[enemy_i].collision.up = 0;
        while (colEnemy < enemyCount)
        {
            if (overlap(enemy[enemy_i], enemy[colEnemy]) &&
                enemy[enemy_i].rect.y - (enemy[colEnemy].rect.y + enemy[colEnemy].rect.h) < enemy[enemy_i].speed && // Dirty fix for a bug I don't understand
                enemy[enemy_i].rect.y >= enemy[colEnemy].rect.y + 0.9 * enemy[colEnemy].rect.h &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.up = 1;
            }

            colEnemy++;
        }
        if (enemy[enemy_i].rect.y + enemy[enemy_i].rect.h / 2 > player.rect.y + player.rect.h / 2 && !enemy[enemy_i].collision.up && enemy[enemy_i].active) // move up
            enemy[enemy_i].rect.y -= enemy[enemy_i].speed + rand() % 2; // + 0 ou + 1

        colEnemy = 0;
        enemy[enemy_i].collision.down = 0;
        while (colEnemy < enemyCount)
        {
            if (overlap(enemy[enemy_i], enemy[colEnemy]) &&
                enemy[enemy_i].rect.y + enemy[enemy_i].rect.h <= enemy[colEnemy].rect.y + 0.1 * enemy[colEnemy].rect.h &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.down = 1;
            }

            colEnemy++;
        }
        if (enemy[enemy_i].rect.y + enemy[enemy_i].rect.h / 2 < player.rect.y + player.rect.h / 2 && !enemy[enemy_i].collision.down && enemy[enemy_i].active) // move down
            enemy[enemy_i].rect.y += enemy[enemy_i].speed + rand() % 2;

        colEnemy = 0;

        if (overlap(enemy[enemy_i], player))
        {
            reset = 1;
        }

        enemy_i++;
    }
}
