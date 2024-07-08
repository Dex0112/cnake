#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

// I need a like constants.h for window dimensions

// Do this first
void move_apple(GameState *game_state);
void render_game_state(GameState *game_state);

void game(SDL_Renderer *renderer);
