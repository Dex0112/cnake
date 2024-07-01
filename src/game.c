#include "entity.h"
#include <SDL2/SDL_render.h>
#include <stdlib.h>

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

#define SNAKE_HEAD_GFX_PATH "../gfx/Snake_Head.png"
#define SNAKE_BODY_GFX_PATH "../gfx/Snake_Body.png"

void append(GameState *gameState) {
    SnakeNode *current = gameState->snake_head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (SnakeNode *)malloc(sizeof(SnakeNode));

    // TODO: handle scaling
    current->next->self = create_entity(
        gameState->renderer, SNAKE_HEAD_GFX_PATH,
        (Vector){current->self->position.x, current->self->position.y},
        (Vector){0, 0});
}

void tick(SnakeNode *head) {
    Vector prev_position = head->self->position;
    // Move head

    head->self->position = (Vector){1, 0};
    // head->self->position = {head->self->position.x, head->self->position.y};

    // Detect collision

    SnakeNode *current = head->next;

    while (current != NULL) {
        current->self->position = prev_position;

        prev_position = current->self->position;

        current = current->next;
    }
}

// Return scene state enum or scene trasition thing
void game() {
}
