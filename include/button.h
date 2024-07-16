#ifndef BUTTON
#define BUTTON

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

typedef struct {
    SDL_Texture *text_texture;
    SDL_Rect rect;
} Button;

Button *create_button(SDL_Renderer *, char *text, SDL_Rect button_rect);

void render_button(SDL_Renderer *, Button);

void free_button(Button *);

#endif
