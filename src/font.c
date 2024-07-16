#include "font.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

TTF_Font *font_regular = NULL;
// TTF_Font *font_large = NULL;

SDL_Color font_color = {255, 255, 255, 255};

void load_font_regular(char *path, unsigned int size) {
    font_regular = TTF_OpenFont(path, size);
}

// Maybe making texture color a parameter
SDL_Texture *load_text(SDL_Renderer *renderer, char *text, TTF_Font *font) {
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, font_color);

    SDL_Texture *text_texture =
        SDL_CreateTextureFromSurface(renderer, text_surface);

    return text_texture;
}
