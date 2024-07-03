#include "game.h"
#include "entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>

#define VECTOR_UP                                                              \
    (Vector) { 0, -1 }
#define VECTOR_LEFT                                                            \
    (Vector) { -1, 0 }
#define VECTOR_RIGHT                                                           \
    (Vector) { 1, 0 }
#define VECTOR_DOWN                                                            \
    (Vector) { 0, 1 }

typedef struct node {
    Entity *self;
    struct node *next;
} SnakeNode;

// keep score/length?
// Scale?
typedef struct {
    unsigned int width;
    unsigned int height;
    Vector current_dir;
    SnakeNode *snake_head;
    Entity *apple;
    SDL_Renderer *renderer;
} GameState;

// Append
// tick

// Return scene state enum or scene trasition thing
void game(SDL_Renderer *renderer) {
    Entity *snake = create_entity(renderer, "./gfx/Snake_Head.png",
                                  (Vector){0, 0}, (Vector){20, 20});

    SDL_Event event;
    bool running = true;
    Vector current_dir = VECTOR_UP;

    int frame_count = 1;
    int tick_speed = 10;

    // Draw grid lines
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                printf("Quiting\n");
            }
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LEFT]) {
            current_dir = VECTOR_LEFT;
        }

        if (state[SDL_SCANCODE_RIGHT]) {
            current_dir = VECTOR_RIGHT;
        }

        if (state[SDL_SCANCODE_UP]) {
            current_dir = VECTOR_UP;
        }

        if (state[SDL_SCANCODE_DOWN]) {
            current_dir = VECTOR_DOWN;
        }

        if (frame_count == 0) {
            snake->position.x += current_dir.x * 24;
            snake->position.y += current_dir.y * 24;
        }

        SDL_SetRenderDrawColor(renderer, 42, 42, 42, 255);
        SDL_RenderClear(renderer);

        render_entity(renderer, snake);

        SDL_RenderPresent(renderer);

        frame_count = (frame_count + 1) % tick_speed;

        SDL_Delay(20);
    }
}
