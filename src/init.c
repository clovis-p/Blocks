#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "main.h"

void init(gameObject *player, gameObject *enemy, gameObject *bullet)
{
    int enemy_i = 0;
    int bullet_i = 0;

    const int min_brightness = 100;

    player->color.r = 0;
    player->color.g = 0;
    player->color.b = 255;

    player->active = 1;

    player->speed = 3;

    player->fp.w = 40.0 / 1280.0 * RESOLUTION_X_F;
    player->fp.h = 40.0 / 720.0 * RESOLUTION_Y_F;
    player->rect.w = player->fp.w;
    player->rect.h = player->fp.h;
    player->rect.x = RESOLUTION_X / 2 - player->rect.w / 2;
    player->rect.y = RESOLUTION_Y / 2 - player->rect.h / 2;

    while (bullet_i < 50)
    {
        bullet[bullet_i].rect.x = 0;
        bullet[bullet_i].rect.y = 0;
        bullet[bullet_i].rect.w = 8.0 / 1280.0 * RESOLUTION_X_F;
        bullet[bullet_i].rect.h = 8.0 / 720.0 * RESOLUTION_Y_F;
        bullet[bullet_i].active = 0;
        bullet[bullet_i].direction = 0;
        bullet[bullet_i].speed = 8.0 / 1280.0 * RESOLUTION_X_F;
        bullet_i++;
    }

    while (enemy_i < ENEMY_COUNT)
    {
        enemy[enemy_i].speed = 1;

        enemy[enemy_i].color.r = rand() % (256 - min_brightness) + min_brightness; // random number, min: min_brightness, max 255
        enemy[enemy_i].color.g = rand() % (256 - min_brightness) + min_brightness;
        enemy[enemy_i].color.b = rand() % (256 - min_brightness) + min_brightness;

        enemy[enemy_i].fp.w = 40.0 / 1280.0 * RESOLUTION_X_F;
        enemy[enemy_i].fp.h = 40.0 / 720.0 * RESOLUTION_Y_F;
        enemy[enemy_i].rect.w = enemy[enemy_i].fp.w;
        enemy[enemy_i].rect.h = enemy[enemy_i].fp.h;

        if (enemy_i == 0)
            enemy[enemy_i].fp.x = 0;
        else if (rand() % 2) // randomize enemy start points (flip a coin, top or bottom)
        {
            enemy[enemy_i].fp.x = RESOLUTION_X_F / ENEMY_COUNT * enemy_i;
            enemy[enemy_i].fp.y = 0;
        }
        else
        {
            enemy[enemy_i].fp.x = RESOLUTION_X_F / ENEMY_COUNT * enemy_i;
            enemy[enemy_i].fp.y = RESOLUTION_Y_F - enemy[enemy_i].fp.h;
        }

        enemy[enemy_i].rect.x = enemy[enemy_i].fp.x;
        enemy[enemy_i].rect.y = enemy[enemy_i].fp.y;
        enemy[enemy_i].active = 1;
        enemy_i++;
    }
}
