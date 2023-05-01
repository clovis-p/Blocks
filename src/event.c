#include <stdio.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "event.h"
#include "bullet.h"

void handleEvents(gameObject *player, gameObject *bullet, SDL_Event event)
{
    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

    static int lockShooting = 0;

    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
    {
        quit = 1;
    }

    if (gameState == 0) {
        if (keyStates[SDL_SCANCODE_LSHIFT])
            player->speed = 6;
        else
            player->speed = 3;

        if (keyStates[SDL_SCANCODE_W] && !player->collision.up) // up
        {
            player->rect.y -= player->speed;
            player->direction = 3; // 0 = undefined, 1 = left, 2 = left/up, 3 = up, 4 = up/right, 5 = right, 6 = right/down, 7 = down, 8 = down/left
        }
        if (keyStates[SDL_SCANCODE_A] && !player->collision.left) {
            player->rect.x -= player->speed;
            player->direction = 1;
        }
        if (keyStates[SDL_SCANCODE_W] && keyStates[SDL_SCANCODE_A]) // up/left
        {
            player->direction = 2;
        }
        if (keyStates[SDL_SCANCODE_S] && !player->collision.down) {
            player->rect.y += player->speed;
            player->direction = 7;
        }
        if (keyStates[SDL_SCANCODE_A] && keyStates[SDL_SCANCODE_S]) // left/down
        {
            player->direction = 8;
        }
        if (keyStates[SDL_SCANCODE_D] && !player->collision.right) {
            player->rect.x += player->speed;
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
        if (keyStates[SDL_SCANCODE_SPACE] && !lockShooting) // shoot bullet
        {
            lockShooting = 1;
            shootBullet(bullet, player);
        }
        if (!keyStates[SDL_SCANCODE_SPACE]) {
            lockShooting = 0;
        }
    }
}
