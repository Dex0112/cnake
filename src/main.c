#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "game.h"

int main() {
    TTF_Init();

    SDL_Init(SDL_INIT_VIDEO);

    srandom(time(NULL));

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("IMG_Init: %s\n", IMG_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Cnake", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                          WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    game(renderer);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();

    SDL_Quit();
}
