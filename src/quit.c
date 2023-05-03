#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "quit.h"

void quitF(SDL_Window **win, SDL_Renderer **ren)
{
    SDL_DestroyWindow(*win);
    SDL_DestroyRenderer(*ren);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}