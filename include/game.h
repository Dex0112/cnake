#ifndef GAME

#define GAME

#include <SDL2/SDL.h>

#define GRID_WIDTH 30
#define GRID_WIDTH 30

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
    Apple apple;
} GameState;

void game(SDL_Renderer *renderer);
#endif
