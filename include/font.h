#ifndef FONT
#define FONT

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font *font_regular;
extern TTF_Font *font_large;

extern const SDL_Color font_color_regular;
extern const SDL_Color font_color_large;

// Possibely have like a font_init to load all these then like a font_destroy or
// something
void load_font_regular(char *path, unsigned int size);
void load_font_large(char *path, unsigned int size);

SDL_Texture *load_text(SDL_Renderer *, char *text, TTF_Font *,
                       SDL_Color font_color);

#endif
