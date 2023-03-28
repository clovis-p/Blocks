#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "main.h"

void handleEvents(gameObject *player, gameObject *bullet, SDL_Event event)
{
    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

    static int i = 0;

    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
        quit = 1;
    }

    if (keyStates[SDL_SCANCODE_LSHIFT])
        speed = 6;
    else
        speed = 3;

    if (keyStates[SDL_SCANCODE_W] && player->rect.y > 0) // up
    {
        player->rect.y -= speed;
        player->direction = 3; // 0 = undefined, 1 = left, 2 = left/up, 3 = up, 4 = up/right, 5 = right, 6 = right/down, 7 = down, 8 = down/left
    }
    if (keyStates[SDL_SCANCODE_A] && player->rect.x > 0)
    {
        player->rect.x -= speed;
        player->direction = 1;
    }
    if (keyStates[SDL_SCANCODE_W] && keyStates[SDL_SCANCODE_A]) // up/left
    {
        player->direction = 2;
    }
    if (keyStates[SDL_SCANCODE_S] && player->rect.y + player->rect.h < RESOLUTION_Y)
    {
        player->rect.y += speed;
        player->direction = 7;
    }
    if (keyStates[SDL_SCANCODE_A] && keyStates[SDL_SCANCODE_S]) // left/down
    {
        player->direction = 8;
    }
    if (keyStates[SDL_SCANCODE_D] && player->rect.x + player->rect.w < RESOLUTION_X)
    {
        player->rect.x += speed;
        player->direction = 5;
    }
    if (keyStates[SDL_SCANCODE_S] && keyStates[SDL_SCANCODE_D]) // down/right
    {
        player->direction = 6;
    }
    if (keyStates[SDL_SCANCODE_D] && keyStates[SDL_SCANCODE_W]) // right/up
    {
        player->direction = 4;
    }
    if (keyStates[SDL_SCANCODE_SPACE] && !shoot) // shoot bullet
    {
        shoot = 1;
        printf("%d ", i);
        bullet[i].direction = player->direction;
        bullet[i].active = 1;
        bullet[i].rect.x = player->rect.x + player->rect.w / 2 - bullet[i].rect.w / 2;
        bullet[i].rect.y = player->rect.y + player->rect.h / 2 - bullet[i].rect.h / 2;

        if (i < 49)
            i++;
        else
            i = 0;
    }
    if (!keyStates[SDL_SCANCODE_SPACE])
    {
        shoot = 0;
    }
}
