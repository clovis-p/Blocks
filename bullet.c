#include <stdio.h>
#include <stdlib.h>

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
