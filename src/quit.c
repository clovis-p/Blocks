#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "quit.h"

void quitF()
{
    IMG_Quit();
    SDL_Quit();
}