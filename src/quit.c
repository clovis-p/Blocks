#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "quit.h"

void quitF(SDL_Window **win, SDL_Renderer **ren, TTF_Font **font)
{
    TTF_CloseFont(*font);

    SDL_DestroyRenderer(*ren);
    SDL_DestroyWindow(*win);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}