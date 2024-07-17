#ifndef GAME

#define GAME

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "button.h"

#define GRID_WIDTH 10
#define GRID_HEIGHT 10
#define TICK_SPEED 25

typedef enum { UP, LEFT, DOWN, RIGHT } Direction;

typedef struct {
    Button *restart_button;
    Button *quit_button;
    SDL_Rect score_rect;
    SDL_Texture *score_text;
    SDL_Rect background;
} EndScreen;

typedef struct {
    SDL_Texture *snake_head;
    SDL_Texture *snake_body;
    SDL_Texture *apple;
} Resources;

typedef struct node {
    struct node *next;
    int x;
    int y;
} SnakeNode;

typedef struct {
    int x;
    int y;
} Apple;

typedef struct {
    unsigned int width;
    unsigned int height;
    SnakeNode *snake;
    Apple *apple;
    Direction direction;
    unsigned int score;
} GameState;

void game(SDL_Renderer *renderer);
#endif
