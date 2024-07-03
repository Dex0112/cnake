#include "game.h"
#include "entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>

#define VECTOR_UP                                                              \
    (Vector) { 0, 1 }
#define VECTOR_LEFT                                                            \
    (Vector) { -1, 0 }
#define VECTOR_RIGHT                                                           \
    (Vector) { 1, 0 }
#define VECTOR_DOWN                                                            \
    (Vector) { 0, -1 }

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
                                  (Vector){250, 250}, (Vector){100, 100});

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                printf("Quiting\n");
            }
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LEFT]) {
            snake->position.x -= 10;
        }

        if (state[SDL_SCANCODE_RIGHT]) {
            snake->position.x += 10;
        }

        if (state[SDL_SCANCODE_UP]) {
            snake->position.y -= 10;
        }

        if (state[SDL_SCANCODE_DOWN]) {
            snake->position.y += 10;
        }

        SDL_SetRenderDrawColor(renderer, 144, 144, 144, 255);
        SDL_RenderClear(renderer);

        render_entity(renderer, snake);

        SDL_RenderPresent(renderer);

        SDL_Delay(20);
    }
}
