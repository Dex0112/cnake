#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "button.h"
#include "constants.h"
#include "font.h"

bool is_game_over(GameState game_state);
bool is_in_bounds(int x, int y, GameState game_state);
bool collides_with_snake(int x, int y, SnakeNode *head);

void tick(GameState *);
void move_apple(GameState *);
void append(SnakeNode *head);

Direction handle_input(GameState *, SDL_Scancode);

EndScreen *create_end_screen(SDL_Renderer *, GameState);

void render_game_state(GameState, Resources, SDL_Renderer *);
void render_end_screen(SDL_Renderer *renderer, EndScreen end_screen);

void free_resources(Resources *);
void free_game_state(GameState);
void free_end_screen(EndScreen *end_screen);

bool game(SDL_Renderer *renderer) {
    Resources resources = {
        IMG_LoadTexture(renderer, "./gfx/Snake_Head.png"),
        IMG_LoadTexture(renderer, "./gfx/Snake_Body.png"),
        IMG_LoadTexture(renderer, "./gfx/Apple.png"),
    };

    GameState game_state = {
        GRID_WIDTH, GRID_HEIGHT, (SnakeNode *)malloc(sizeof(SnakeNode)),
        NULL,       UP,          0,
    };

    Direction next_direction = game_state.direction;

    game_state.snake->x = game_state.width / 2 - 1;
    game_state.snake->y = game_state.height / 2 - 1;
    game_state.snake->next = NULL;

    SDL_SetRenderDrawColor(renderer, 144, 144, 144, 255);
    SDL_RenderClear(renderer);

    render_game_state(game_state, resources, renderer);

    SDL_RenderPresent(renderer);

    render_game_state(game_state, resources, renderer);

    SDL_RenderPresent(renderer);

    EndScreen *end_screen = NULL;

    bool replay = false;

    // Negative 1 so it will wait for input
    int frame_count = -1;
    bool running = true;
    bool game_over = false;
    SDL_Event event;

    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
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
                    next_direction = handle_input(&game_state, event.key.keysym.scancode);

                    if (frame_count == -1) {
                        frame_count = 0;

                        game_state.apple = (Apple *)malloc(sizeof(Apple));
                        game_state.apple->x = 0;
                        game_state.apple->y = 0;

                        move_apple(&game_state);
                    }

                    break;
                case SDL_MOUSEBUTTONUP:
                    if (!game_over) break;

                    int mouse_x, mouse_y;

                    SDL_GetMouseState(&mouse_x, &mouse_y);

                    if (collide_point(*end_screen->quit_button, mouse_x,
                                      mouse_y)) {
                        running = false;
                    }

                    if (collide_point(*end_screen->restart_button, mouse_x,
                                      mouse_y)) {
                        replay = true;
                        running = false;
                    }

                    break;
            }
        }

        if (frame_count >= TICK_SPEED) {
            frame_count = 0;

            if (!game_over) {
                game_state.direction = next_direction;
                tick(&game_state);
            }

            game_over = is_game_over(game_state);

            SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
            SDL_RenderClear(renderer);

            render_game_state(game_state, resources, renderer);

            if (game_over) {
                if (end_screen == NULL) {
                    end_screen = create_end_screen(renderer, game_state);
                }

                render_end_screen(renderer, *end_screen);
            }

            SDL_RenderPresent(renderer);
        }

        if (frame_count >= 0) frame_count++;

        SDL_Delay(10);
    }

    free_resources(&resources);
    free_game_state(game_state);
    free_end_screen(end_screen);

    return replay;
}

bool is_game_over(GameState game_state) {
    SnakeNode *head = game_state.snake;

    if (!is_in_bounds(head->x, head->y, game_state)) {
        return true;
    }

    if (collides_with_snake(head->x, head->y, head->next)) {
        return true;
    }

    return false;
}

bool is_in_bounds(int x, int y, GameState game_state) {
    if (x < 0) {
        return false;
    }

    if (y < 0) {
        return false;
    }

    if (x >= game_state.width) {
        return false;
    }

    if (y >= game_state.height) {
        return false;
    }

    return true;
}

bool collides_with_snake(int x, int y, SnakeNode *head) {
    SnakeNode *current = head;

    while (current) {
        if (x == current->x && y == current->y) {
            return true;
        }

        current = current->next;
    }

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
        game_state->score++;
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

void render_end_screen(SDL_Renderer *renderer, EndScreen end_screen) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &end_screen.background);

    SDL_RenderCopy(renderer, end_screen.score_text, NULL,
                   &end_screen.score_rect);

    render_button(renderer, *end_screen.restart_button);
    render_button(renderer, *end_screen.quit_button);
}

// There is a bug where you can go the opposit direction by hitting and adjacent
// direction then before the next tic pressing the opposit of the previous
// direction
Direction handle_input(GameState *game_state, SDL_Scancode key) {
    switch (key) {
        case SDL_SCANCODE_UP:
            if (game_state->direction == DOWN) {
                break;
            }

            return UP;
            break;
        case SDL_SCANCODE_DOWN:
            if (game_state->direction == UP) {
                break;
            }

            return DOWN;
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

            return RIGHT;
            break;
        default:
            break;
    }

    return game_state->direction;
}

EndScreen *create_end_screen(SDL_Renderer *renderer, GameState game_state) {
    EndScreen *end_screen = (EndScreen *)malloc(sizeof(EndScreen));

    char score_result[25];

    sprintf(score_result, "Score: %d", game_state.score);

    // Add a Boarder??
    end_screen->background = (SDL_Rect){
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2,
        WINDOW_WIDTH * 0.75,
        WINDOW_HEIGHT * 0.75,
    };

    end_screen->background.x -= end_screen->background.w / 2;
    end_screen->background.y -= end_screen->background.h / 2;

    end_screen->restart_button =
        create_button(renderer, "Restart",
                      (SDL_Rect){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 100, 50});

    end_screen->quit_button = create_button(
        renderer, "Quit",
        (SDL_Rect){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 75, 75, 50});

    end_screen->score_text =
        load_text(renderer, score_result, font_large, font_color_large);

    SDL_QueryTexture(end_screen->score_text, NULL, NULL,
                     &(end_screen->score_rect.w), &(end_screen->score_rect.h));

    end_screen->score_rect.x = WINDOW_WIDTH / 2;
    end_screen->score_rect.x -= end_screen->score_rect.w / 2;

    end_screen->score_rect.y = WINDOW_HEIGHT / 2 - 75;
    end_screen->score_rect.y -= end_screen->score_rect.h / 2;

    return end_screen;
}

void free_resources(Resources *resources) {
    SDL_DestroyTexture(resources->apple);
    SDL_DestroyTexture(resources->snake_head);
    SDL_DestroyTexture(resources->snake_body);
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

void free_end_screen(EndScreen *end_screen) {
    if (end_screen == NULL) return;

    SDL_DestroyTexture(end_screen->score_text);

    free_button(end_screen->restart_button);
    free_button(end_screen->quit_button);
}
