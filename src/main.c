#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"
#include "init.h"
#include "collision.h"
#include "event.h"
#include "bullet.h"
#include "render.h"
#include "quit.h"

int quit;
int shoot;
int reset;

int enemyCount;

int gameState; // 0: Normal in-game state, 1: Paused game state (not implemented yet), 2: Level clear state
               // 3: Game over state

size window_res;

int main(int argc, char **argv)
{
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;

    TTF_Font *font = NULL;

    if (!init(&win, SDL_WINDOW_SHOWN, &ren, &font))
    {
        return -1;
    }

    /* The following section is only for debugging
     *
     *  I'm having trouble getting SDL to give me the resolution of a single monitor in a multi-monitor linux setup.
     *  SDL_GetNumVideoDisplays returns 1, although I have 2 monitors. Getting the resolution with SDL_GetDisplayMode
     *  gives the resolution of all displays combined, which isn't what we want.
     */

    static int display_in_use = 0; // Only using first display

    int i, display_mode_count;
    SDL_DisplayMode mode;
    Uint32 f;

    SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        return 1;
    }
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return 1;
        }
        f = mode.format;

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i",
                i, SDL_BITSPERPIXEL(f),
                SDL_GetPixelFormatName(f),
                mode.w, mode.h);
    }

    // end

    SDL_Event event;

    enemyCount = 1;

    gameObject player;
    gameObject bullet[51];
    int bullet_i = 0;
    int enemy_i = 0;
    gameObject enemy[75];
    SDL_Texture *enemyTexture[75];

    quit = 0;
    shoot = 0;

    initGame(ren, &player, &enemy, &enemyTexture, &bullet);

    reset = 0;
    int remainingEnemies = enemyCount;

    uint32_t previousTicks = 0;
    uint32_t currentTicks = 0;

    uint32_t resetTimerPreviousTicks = 0;

    gameState = 0; // 0: Normal in-game state, 1: Paused game state (not implemented yet), 2: Game over state

    int lol;

    while (!quit)
    {
        if (!remainingEnemies && gameState == 0)
            reset = 1;

        if (reset)
        {
            if (!remainingEnemies)
            {
                gameState = 2;
            }
            else
            {
                gameState = 3;
            }
            resetTimerPreviousTicks = SDL_GetTicks();
            reset = 0;
        }

        if (gameState == 0)
        {
            checkIfWithinBounds(&player, SET_COLLISION);

            checkEnemyCollisions(&enemy, player); // check collisions and update enemy pos

            updateBulletsPos(&bullet);
        }
        else if (gameState == 1)
        {

        }
        else if (gameState == 2)
        {
            if (currentTicks - resetTimerPreviousTicks < 2000)
            {
                lol = 1;
            }
            else if (lol == 1)
            {
                bullet_i = 0;
                enemy_i = 0;
                initGame(ren, &player, &enemy, &enemyTexture, &bullet);
                gameState = 0;
                lol = 0;
            }

            updateBulletsPos(&bullet);
        }
        else if (gameState == 3)
        {
            if (currentTicks - resetTimerPreviousTicks < 3000)
            {
                lol = 1;
            }
            else if (lol == 1)
            {
                bullet_i = 0;
                enemy_i = 0;
                initGame(ren, &player, &enemy, &enemyTexture, &bullet);
                gameState = 0;
                lol = 0;
            }

            updateBulletsPos(&bullet);
        }
        /*  About the "lol" variable: I do not understand why this is needed, but apparently it is, otherwise the 2000ms
         *  delay does not happen. Removing "gameState = 0;" makes the delay work for some reason, but it is necessary
         *  to set it to zero to restart the game. */

        handleEvents(&player, &bullet, event);

        while (bullet_i < 50 && !reset) // apply damage to shot enemies and count remaining enemies
        {
            enemy_i = 0;
            remainingEnemies = 0;
            while (enemy_i < enemyCount)
            {
                if (enemy[enemy_i].active)
                {
                    checkIfWithinBounds(&bullet[bullet_i], SET_INACTIVE);

                    if (bullet[bullet_i].active && overlap(bullet[bullet_i], enemy[enemy_i]))
                    {
                        enemy[enemy_i].fp.w -= 1.0;
                        enemy[enemy_i].fp.h -= 1.0;
                        enemy[enemy_i].rect.w = enemy[enemy_i].fp.w / 1280 * window_res.fw;
                        enemy[enemy_i].rect.h = enemy[enemy_i].fp.h / 720 * window_res.fh;
                        if (enemy[enemy_i].fp.w < 10)
                        {
                            enemy[enemy_i].active = 0;
                        }
                    }
                    remainingEnemies++;
                }
                enemy_i++;
            }
            enemy_i = 0;

            bullet_i++;
        }
        bullet_i = 0;

        render(win, ren, &font, player, enemy, enemyTexture, bullet);

        currentTicks = SDL_GetTicks();

        if (currentTicks - previousTicks < 10)
            SDL_Delay(10 - (currentTicks - previousTicks));  // Maintain a constant frame rate

        previousTicks = SDL_GetTicks();
    }

    quitF(&win, &ren, &font);
    return 0;
}
