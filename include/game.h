#ifndef GAME

#define GAME

#include <SDL2/SDL.h>

typedef struct {
    // width
    // height
    // I really don't think having a struct for position is required but I think
    // that maybe having a struct for apple with just x and y would be fine and
    // simple apple || apple x & y snake head
} GameState;

typedef struct node {
    struct node *next;
    // x and y are more like indecies that will be converted to coordinates at
    // rendering
    int x;
    int y;
} SnakeNode;

void game(SDL_Renderer *renderer);
#endif
