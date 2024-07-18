#include "font.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

TTF_Font *font_regular;
TTF_Font *font_large;

const SDL_Color font_color_regular = {255, 255, 255, 255};
const SDL_Color font_color_large = {255, 255, 255, 255};

void load_font_regular(char *path, unsigned int size) {
    font_regular = TTF_OpenFont(path, size);
}

void load_font_large(char *path, unsigned int size) {
    font_large = TTF_OpenFont(path, size);
}

// Maybe making texture color a parameter
SDL_Texture *load_text(SDL_Renderer *renderer, char *text, TTF_Font *font,
                       SDL_Color font_color) {
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, font_color);

    SDL_Texture *text_texture =
        SDL_CreateTextureFromSurface(renderer, text_surface);

    if (text_texture == NULL) {
        printf("Text is not being loaded properly\n");
    }

    return text_texture;
}
