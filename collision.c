#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "main.h"

int checkCollision(gameObject o1, gameObject o2)
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
