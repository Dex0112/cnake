#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entity.h"

#define VECTOR_UP \
    (Vector) { 0, -1 }
#define VECTOR_LEFT \
    (Vector) { -1, 0 }
#define VECTOR_RIGHT \
    (Vector) { 1, 0 }
#define VECTOR_DOWN \
    (Vector) { 0, 1 }

typedef struct node {
    Entity *self;
    struct node *next;
} SnakeNode;

// keep score/length?
// Scale?
typedef struct {
    unsigned int grid_width;
    unsigned grid_height;
    // Maybe implement
    // bool screen_wrap;
    Vector current_dir;
    SnakeNode *snake_head;
    Entity *apple;
    SDL_Renderer *renderer;
} GameState;

// Append
// tick

void tick(GameState *game_state, Vector input_dir) {
    // Check if the snake is going to collide with the apple
    // If it does
    // append to snake
    // place apple elsewhere

    Vector prev_position = game_state->snake_head->self->transform.position;
    // Move snake head

    game_state->snake_head->self->transform.position.x += input_dir.x * 72;
    game_state->snake_head->self->transform.position.y += input_dir.y * 72;

    SnakeNode *current = game_state->snake_head->next;
    while (current != NULL) {
        Vector current_position = current->self->transform.position;

        current->self->transform.position = prev_position;
        prev_position = current_position;
    }
}

void render_game_state(GameState *game_state) {
    // Maybe render grid lines
    // Render Snake
    SnakeNode *current = game_state->snake_head;

    while (current != NULL) {
        render_entity(game_state->renderer, current->self);

        current = current->next;
    }
}

// Return scene state enum or scene trasition thing
void game(SDL_Renderer *renderer) {
    // Window Width and Height 720
    GameState game_state = {10,
                            10,
                            VECTOR_UP,
                            (SnakeNode *)malloc(sizeof(SnakeNode)),
                            create_entity(renderer, "./gfx/Apple.png",
                                          (Vector){0, 0}, (Vector){1, 1}),
                            renderer};

    game_state.snake_head->self = create_entity(
        renderer, "./gfx/Snake_Head.png", (Vector){36, 36}, (Vector){72, 72});

    game_state.snake_head->next = NULL;

    SDL_Event event;
    bool running = true;
    Vector current_dir = VECTOR_UP;

    int frame_count = -1;
    int tick_speed = 25;

    // Draw grid lines
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                printf("Quiting\n");
            } else if (event.type == SDL_KEYDOWN) {
                if (frame_count < 0) {
                    frame_count = 1;
                }

                // Parse input function?
                // Like parse_input(input, &current_dir)
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    current_dir = VECTOR_LEFT;
                } else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    current_dir = VECTOR_RIGHT;
                } else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    current_dir = VECTOR_UP;
                } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    current_dir = VECTOR_DOWN;
                }
            }
        }

        if (frame_count == 0) {
            // Movement
            tick(&game_state, current_dir);
        }

        SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
        SDL_RenderClear(renderer);

        // Render Game State
        render_game_state(&game_state);

        SDL_RenderPresent(renderer);

        if (frame_count >= 0) {
            frame_count = (frame_count + 1) % tick_speed;
        }

        SDL_Delay(20);
    }
}
