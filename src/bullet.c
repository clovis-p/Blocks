#include <stdio.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "bullet.h"

void shootBullet(gameObject *bullet, gameObject *shooter)
{
    static int i = 0;

    bullet[i].direction = shooter->direction;
    bullet[i].active = 1;
    bullet[i].rect.x = shooter->rect.x + shooter->rect.w / 2 - bullet[i].rect.w / 2;
    bullet[i].rect.y = shooter->rect.y + shooter->rect.h / 2 - bullet[i].rect.h / 2;

    if (i < 49)
        i++;
    else
        i = 0;
}

void updateBulletsPos(gameObject *bullet)
{
    int i = 0;
    while (i < 50)
    {
        if (bullet[i].active)
        {   // 0 = undefined, 1 = left, 2 = left/up, 3 = up, 4 = up/right, 5 = right, 6 = right/down, 7 = down, 8 = down/left
            if (bullet[i].direction == 1 || bullet[i].direction == 0)
            {
                bullet[i].rect.x -= bullet[i].speed;
            }
            if (bullet[i].direction == 2)
            {
                bullet[i].rect.x -= bullet[i].speed;
                bullet[i].rect.y -= bullet[i].speed;
            }
            if (bullet[i].direction == 3)
            {
                bullet[i].rect.y -= bullet[i].speed;
            }
            if (bullet[i].direction == 4)
            {
                bullet[i].rect.y -= bullet[i].speed;
                bullet[i].rect.x += bullet[i].speed;
            }
            if (bullet[i].direction == 5)
            {
                bullet[i].rect.x += bullet[i].speed;
            }
            if (bullet[i].direction == 6)
            {
                bullet[i].rect.x += bullet[i].speed;
                bullet[i].rect.y += bullet[i].speed;
            }
            if (bullet[i].direction == 7)
            {
                bullet[i].rect.y += bullet[i].speed;
            }
            if (bullet[i].direction == 8)
            {
                bullet[i].rect.y += bullet[i].speed;
                bullet[i].rect.x -= bullet[i].speed;
            }
        }
        i++;
    }
}
