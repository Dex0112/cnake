#include "button.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "font.h"

SDL_Color button_color = {};

Button *create_button(SDL_Renderer *renderer, char *text,
                      SDL_Rect button_rect) {
    Button *button = (Button *)malloc(sizeof(Button));

    button->text_texture = load_text(renderer, text, font_regular);
    button->rect = button_rect;

    return button;
}

void render_button(SDL_Renderer *renderer, Button button) {
    SDL_SetRenderDrawColor(renderer, button_color.a, button_color.g,
                           button_color.b, button_color.a);

    button.rect.x += button.rect.w / 2;
    button.rect.y += button.rect.h / 2;

    SDL_RenderDrawRect(renderer, &button.rect);

    SDL_Rect text_rect;

    SDL_QueryTexture(button.text_texture, NULL, NULL, &text_rect.w,
                     &text_rect.h);
    text_rect.x = button.rect.x + text_rect.w / 2;
    text_rect.y = button.rect.y + text_rect.h / 2;

    SDL_RenderCopy(renderer, button.text_texture, NULL, &text_rect);
}

void free_button(Button *button) {
    free(button->text_texture);
    free(button);
}
