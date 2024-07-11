#ifndef GAME

#define GAME

#include <SDL2/SDL.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 10
#define TICK_SPEED 25

typedef enum { UP, LEFT, DOWN, RIGHT } Direction;

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
} GameState;

void game(SDL_Renderer *renderer);
#endif
