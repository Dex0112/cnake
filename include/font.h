#ifndef FONT
#define FONT

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font *font_regular;
extern SDL_Color font_color;

void load_font_regular(char *path, unsigned int size);
SDL_Texture *load_text(SDL_Renderer *renderer, char *text, TTF_Font *font);

#endif
