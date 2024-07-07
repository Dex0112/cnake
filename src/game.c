#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
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
#define VECTOR_ZERO \
    (Vector) { 0, 0 }

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

void move_apple(GameState *game_state) {
    game_state->apple->transform.position.x =
        random() % game_state->grid_width * 72 + 36;
    game_state->apple->transform.position.y =
        random() % game_state->grid_height * 72 + 36;
}

void append(SnakeNode *head, SDL_Renderer *renderer) {
    SnakeNode *current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    printf("Found the end of the snake\n");

    SnakeNode *node = (SnakeNode *)malloc(sizeof(SnakeNode));
    node->self =
        create_entity(renderer, "./gfx/Snake_Body.png",
                      current->self->transform.position, (Vector){72, 72});

    printf("Created Entity\n");

    node->next = NULL;

    current->next = node;
}

void tick(GameState *game_state, Vector input_dir) {
    // Check if the snake is going to collide with the apple
    // If it does
    // append to snake
    // place apple elsewhere

    Vector prev_position = game_state->snake_head->self->transform.position;

    game_state->snake_head->self->transform.position.x += input_dir.x * 72;
    game_state->snake_head->self->transform.position.y += input_dir.y * 72;

    if (game_state->snake_head->self->transform.position.x ==
        game_state->apple->transform.position.x) {
        if (game_state->snake_head->self->transform.position.y ==
            game_state->apple->transform.position.y) {
            move_apple(game_state);
            append(game_state->snake_head, game_state->renderer);
        }
    }

    SnakeNode *current = game_state->snake_head->next;
    while (current != NULL) {
        Vector current_position = current->self->transform.position;

        current->self->transform.position = prev_position;
        prev_position = current_position;

        current = current->next;
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

    if (game_state->apple) {
        render_entity(game_state->renderer, game_state->apple);
    }
}

// Return scene state enum or scene trasition thing
void game(SDL_Renderer *renderer) {
    // Window Width and Height 720

    // This should only be called once so I need to put this in main or
    // something
    srandom(time(NULL));

    GameState game_state = {10,        10,
                            VECTOR_UP, (SnakeNode *)malloc(sizeof(SnakeNode)),
                            NULL,      renderer};

    // Find a way to center the snake
    game_state.snake_head->self = create_entity(
        renderer, "./gfx/Snake_Head.png", (Vector){36, 36}, (Vector){72, 72});

    game_state.snake_head->next = NULL;

    SDL_Event event;
    bool running = true;
    Vector current_dir = VECTOR_ZERO;

    int frame_count = -1;
    int tick_speed = 25;

    // Draw grid lines
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                printf("Quiting\n");
            } else if (event.type == SDL_KEYDOWN) {
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

                if (frame_count < 0) {
                    if (current_dir.x == VECTOR_ZERO.x &&
                        current_dir.y == VECTOR_ZERO.y) {
                        continue;
                    }

                    frame_count = 1;

                    game_state.apple =
                        create_entity(renderer, "./gfx/Apple.png",
                                      (Vector){0, 0}, (Vector){72, 72});

                    printf("Moving Apple\n");
                    move_apple(&game_state);
                }
            }
        }

        if (frame_count <= 0) {
            if (frame_count != -1) {
                tick(&game_state, current_dir);
            }
            // Render Game State
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderClear(renderer);
            render_game_state(&game_state);
            SDL_RenderPresent(renderer);
        }

        if (frame_count >= 0) {
            frame_count = (frame_count + 1) % tick_speed;
        }

        SDL_Delay(20);
    }
}
