#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

typedef struct {
} GameState;

// Do this first
// I need a like constants.h for window dimensions
void render_game_state(GameState *game_state);

void game(SDL_Renderer *renderer) {
}
