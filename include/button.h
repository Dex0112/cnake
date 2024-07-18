#ifndef BUTTON
#define BUTTON

#include <stdbool.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

typedef struct {
    SDL_Texture *text_texture;
    SDL_Rect rect;
} Button;

bool collide_point(Button button, int x, int y);

Button *create_button(SDL_Renderer *, char *text, SDL_Rect button_rect);

void render_button(SDL_Renderer *, Button);

void free_button(Button *);

#endif
