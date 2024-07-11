#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

// Do this first
bool collides_with_snake(int x, int y, SnakeNode *head);

void tick(GameState *);
void move_apple(GameState *);
void append(SnakeNode *head);

void handle_input(GameState *, SDL_Scancode);

void render_game_state(GameState, Resources, SDL_Renderer *);

void free_game_state(GameState);

void game(SDL_Renderer *renderer) {
    Resources resources = {
        IMG_LoadTexture(renderer, "./gfx/Snake_Head.png"),
        IMG_LoadTexture(renderer, "./gfx/Snake_Body.png"),
        IMG_LoadTexture(renderer, "./gfx/Apple.png"),
    };

    GameState game_state = {
        GRID_WIDTH,
        GRID_HEIGHT,
        (SnakeNode *)malloc(sizeof(SnakeNode)),
        (Apple *)malloc(sizeof(Apple)),
        UP,
    };

    game_state.snake->x = game_state.width / 2;
    game_state.snake->y = game_state.height / 2;
    game_state.snake->next = NULL;

    // Don't render apple before input
    // game_state.apple = NULL;

    game_state.apple = (Apple *)malloc(sizeof(Apple));
    game_state.apple->x = 0;
    game_state.apple->y = 0;

    move_apple(&game_state);

    int frame_count = 0;
    bool running = true;
    SDL_Event event;

    SDL_SetRenderDrawColor(renderer, 144, 144, 144, 255);
    SDL_RenderClear(renderer);

    render_game_state(game_state, resources, renderer);

    SDL_RenderPresent(renderer);

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    handle_input(&game_state, event.key.keysym.scancode);
                    break;
            }
        }

        if (frame_count >= TICK_SPEED) {
            frame_count = 0;

            tick(&game_state);

            SDL_SetRenderDrawColor(renderer, 144, 144, 144, 255);
            SDL_RenderClear(renderer);

            render_game_state(game_state, resources, renderer);

            SDL_RenderPresent(renderer);
        }

        frame_count++;
        SDL_Delay(20);
    }

    free_game_state(game_state);
}

bool collides_with_snake(int x, int y, SnakeNode *head) {
    SnakeNode *current = head;

    while (current) {
        if (x == current->x && y == current->y) {
            return true;
            printf("Collision detected: %d %d\n", x, y);
        }

        current = current->next;
    }

    printf("No collision detected %d %d\n", x, y);

    return false;
}

void tick(GameState *game_state) {
    int prev_x = game_state->snake->x;
    int prev_y = game_state->snake->y;

    switch (game_state->direction) {
        case UP:
            game_state->snake->y -= 1;
            break;
        case DOWN:
            game_state->snake->y += 1;
            break;
        case LEFT:
            game_state->snake->x -= 1;
            break;
        case RIGHT:
            game_state->snake->x += 1;
            break;
    }

    bool apple_collected = game_state->snake->x == game_state->apple->x &&
                           game_state->snake->y == game_state->apple->y;

    if (apple_collected) {
        append(game_state->snake);
    }

    SnakeNode *current = game_state->snake->next;

    while (current) {
        int current_x = current->x;
        int current_y = current->y;

        current->x = prev_x;
        current->y = prev_y;

        prev_x = current_x;
        prev_y = current_y;

        current = current->next;
    }

    if (apple_collected) {
        move_apple(game_state);
    }
}

void move_apple(GameState *game_state) {
    int x = random() % game_state->width;
    int y = random() % game_state->height;

    if (collides_with_snake(x, y, game_state->snake)) {
        move_apple(game_state);
        return;
    }

    game_state->apple->x = x;
    game_state->apple->y = y;
}

void append(SnakeNode *head) {
    SnakeNode *current = head;

    while (current->next) {
        current = current->next;
    }

    SnakeNode *node = (SnakeNode *)malloc(sizeof(SnakeNode));

    node->next = NULL;
    node->x = current->x;
    node->y = current->y;

    current->next = node;
}

void render_game_state(GameState game_state, Resources resources,
                       SDL_Renderer *renderer) {
    int cell_size_x = WINDOW_WIDTH / game_state.width;
    int cell_size_y = WINDOW_HEIGHT / game_state.height;

    SDL_Rect head_rect = {
        game_state.snake->x * cell_size_x,
        game_state.snake->y * cell_size_y,
        cell_size_x,
        cell_size_y,
    };

    SDL_Point head_center = {head_rect.w / 2, head_rect.h / 2};

    double head_angle;

    switch (game_state.direction) {
        case UP:
            head_angle = 0;
            break;
        case DOWN:
            head_angle = 180;
            break;
        case LEFT:
            head_angle = -90;
            break;
        case RIGHT:
            head_angle = 90;
            break;
    }

    SDL_RenderCopyEx(renderer, resources.snake_head, NULL, &head_rect,
                     head_angle, &head_center, SDL_FLIP_NONE);

    SnakeNode *current = game_state.snake->next;

    while (current != NULL) {
        SDL_Rect body_rect = {
            current->x * cell_size_x,
            current->y * cell_size_y,
            cell_size_x,
            cell_size_y,
        };

        SDL_RenderCopy(renderer, resources.snake_body, NULL, &body_rect);

        current = current->next;
    }

    // Render Apple
    if (game_state.apple == NULL) {
        return;
    }

    SDL_Rect apple_rect = {
        game_state.apple->x * cell_size_x,
        game_state.apple->y * cell_size_y,
        cell_size_x,
        cell_size_y,
    };

    SDL_RenderCopy(renderer, resources.apple, NULL, &apple_rect);
}

void handle_input(GameState *game_state, SDL_Scancode key) {
    switch (key) {
        case SDL_SCANCODE_UP:
            if (game_state->direction == DOWN) {
                break;
            }

            game_state->direction = UP;
            break;
        case SDL_SCANCODE_DOWN:
            if (game_state->direction == UP) {
                break;
            }

            game_state->direction = DOWN;
            break;
        case SDL_SCANCODE_LEFT:
            if (game_state->direction == RIGHT) {
                break;
            }

            game_state->direction = LEFT;
            break;
        case SDL_SCANCODE_RIGHT:
            if (game_state->direction == LEFT) {
                break;
            }

            game_state->direction = RIGHT;
            break;
        default:
            break;
    }
}

void free_game_state(GameState game_state) {
    SnakeNode *current = game_state.snake;

    while (current) {
        SnakeNode *next = current->next;

        free(current);

        current = next;
    }

    free(game_state.apple);
}
