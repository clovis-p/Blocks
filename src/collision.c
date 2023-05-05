#include <stdlib.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "collision.h"

extern int enemyCount;

int overlap(gameObject o1, gameObject o2)
{
    float o1top = o1.fp.y;
    float o1left = o1.fp.x;
    float o1bottom = o1.fp.y + o1.fp.h;
    float o1right = o1.fp.x + o1.fp.w;

    float o2top = o2.fp.y;
    float o2left = o2.fp.x;
    float o2bottom = o2.fp.y + o2.fp.h;
    float o2right = o2.fp.x + o2.fp.w;

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
    if (o->fp.x <= 0)
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
    if (o->fp.x + o->fp.w >= 1280.0)
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
    if (o->fp.y <= 0)
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
    if (o->fp.y + o->fp.h >= 720.0)
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
                enemy[enemy_i].fp.x >= enemy[colEnemy].fp.x + 0.9 * enemy[colEnemy].fp.w &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.left = 1;
            }
            colEnemy++;
        }
        if (enemy[enemy_i].fp.x + enemy[enemy_i].fp.w / 2.0 > player.fp.x + player.fp.w / 2.0 && !enemy[enemy_i].collision.left && enemy[enemy_i].active) // move left
        {
            enemy[enemy_i].fp.x -= enemy[enemy_i].speed + rand() % 2;
        }

        colEnemy = 0;
        enemy[enemy_i].collision.right = 0;
        while (colEnemy < enemyCount)
        {
            if (overlap(enemy[enemy_i], enemy[colEnemy])&&
                enemy[enemy_i].fp.x + enemy[enemy_i].fp.w <= enemy[colEnemy].fp.x + 0.1 * enemy[colEnemy].fp.w &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.right = 1;
            }

            colEnemy++;
        }
        if (enemy[enemy_i].fp.x + enemy[enemy_i].fp.w / 2.0 < player.fp.x + player.fp.w / 2.0 && !enemy[enemy_i].collision.right && enemy[enemy_i].active) // move right
        {
            enemy[enemy_i].fp.x += enemy[enemy_i].speed + rand() % 2;
        }

        colEnemy = 0;
        enemy[enemy_i].collision.up = 0;
        while (colEnemy < enemyCount)
        {
            if (overlap(enemy[enemy_i], enemy[colEnemy]) &&
                enemy[enemy_i].fp.y - (enemy[colEnemy].fp.y + enemy[colEnemy].fp.h) < enemy[enemy_i].speed && // Dirty fix for a bug I don't understand
                enemy[enemy_i].fp.y >= enemy[colEnemy].fp.y + 0.9 * enemy[colEnemy].fp.h &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.up = 1;
            }

            colEnemy++;
        }
        if (enemy[enemy_i].fp.y + enemy[enemy_i].fp.h / 2.0 > player.fp.y + player.fp.h / 2.0 && !enemy[enemy_i].collision.up && enemy[enemy_i].active) // move up
        {
            enemy[enemy_i].fp.y -= enemy[enemy_i].speed + rand() % 2;
        }

        colEnemy = 0;
        enemy[enemy_i].collision.down = 0;
        while (colEnemy < enemyCount)
        {
            if (overlap(enemy[enemy_i], enemy[colEnemy]) &&
                enemy[enemy_i].fp.y + enemy[enemy_i].fp.h <= enemy[colEnemy].fp.y + 0.1 * enemy[colEnemy].fp.h &&
                colEnemy != enemy_i)
            {
                enemy[enemy_i].collision.down = 1;
            }

            colEnemy++;
        }
        if (enemy[enemy_i].fp.y + enemy[enemy_i].fp.h / 2.0 < player.fp.y + player.fp.h / 2.0 && !enemy[enemy_i].collision.down && enemy[enemy_i].active) // move down
        {
            enemy[enemy_i].fp.y += enemy[enemy_i].speed + rand() % 2;
        }

        colEnemy = 0;

        if (overlap(enemy[enemy_i], player))
        {
            reset = 1;
        }

        enemy[enemy_i].rect.x = enemy[enemy_i].fp.x / 1280 * RESOLUTION_X_F;
        enemy[enemy_i].rect.x = enemy[enemy_i].fp.x / 1280 * RESOLUTION_X_F;
        enemy[enemy_i].rect.y = enemy[enemy_i].fp.y / 720 * RESOLUTION_Y_F;
        enemy[enemy_i].rect.y = enemy[enemy_i].fp.y / 720 * RESOLUTION_Y_F;

        enemy_i++;
    }
}
