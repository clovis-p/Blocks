#include <stdio.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "event.h"
#include "bullet.h"

extern int gameState;

extern size window_res;

static int f11_pressed;

void handleEvents(gameObject *player, gameObject *bullet, SDL_Event event)
{
    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

    static int lockShooting = 0;    // These make stuff happen only once
    static int lockPause = 0;       // until it's associated key is released
    static int lockFullscreen = 0;  //

    SDL_PollEvent(&event);

    player->rect.x = player->fp.x / 1280 * window_res.fw;
    player->rect.y = player->fp.y / 720 * window_res.fh;
    player->rect.w = player->fp.w / 1280 * window_res.fw;
    player->rect.h = player->fp.h / 720 * window_res.fh;

    if (event.type == SDL_QUIT)
    {
        quit = 1;
    }

    if (keyStates[SDL_SCANCODE_LALT] && keyStates[SDL_SCANCODE_F4])
    {
        quit = 1;
    }

    if (keyStates[SDL_SCANCODE_F11] && !lockFullscreen) {
        f11_pressed = 1;
        lockFullscreen = 1;
    }
    if (!keyStates[SDL_SCANCODE_F11]) {
        lockFullscreen = 0;
    }

    if (gameState == 0) {
        if (keyStates[SDL_SCANCODE_LSHIFT])
            player->speed = 3.0 * 2.0;
        else
            player->speed = 3.0;

        if (keyStates[SDL_SCANCODE_W] && !player->collision.up) // up
        {
            player->fp.y -= player->speed;
            player->direction = 3; // 0 = undefined, 1 = left, 2 = left/up, 3 = up, 4 = up/right, 5 = right, 6 = right/down, 7 = down, 8 = down/left
        }
        if (keyStates[SDL_SCANCODE_A] && !player->collision.left) {
            player->fp.x -= player->speed;
            player->direction = 1;
        }
        if (keyStates[SDL_SCANCODE_W] && keyStates[SDL_SCANCODE_A]) // up/left
        {
            player->direction = 2;
        }
        if (keyStates[SDL_SCANCODE_S] && !player->collision.down) {
            player->fp.y += player->speed;
            player->direction = 7;
        }
        if (keyStates[SDL_SCANCODE_A] && keyStates[SDL_SCANCODE_S]) // left/down
        {
            player->direction = 8;
        }
        if (keyStates[SDL_SCANCODE_D] && !player->collision.right) {
            player->fp.x += player->speed;
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
    if (gameState == 0 || gameState == 1)
    {
        if (keyStates[SDL_SCANCODE_ESCAPE] && !lockPause) {
            lockPause = 1;
            if (gameState == 0) {
                gameState = 1;
            }
            else if (gameState == 1) {
                gameState = 0;
            }
        }
        if (!keyStates[SDL_SCANCODE_ESCAPE]) {
            lockPause = 0;
        }
    }
}

int fullscreenToggleRequested()
{
    if (f11_pressed)
    {
        f11_pressed = 0;
        return 1;
    }
    else
        return 0;
}
