#include <SDL2/SDL_render.h>
typedef struct {
    int x;
    int y;
} Vector;

typedef struct {
    Vector position;
    Vector size;
    SDL_Texture *texture;
} Entity;

Entity *create_entity(SDL_Renderer *renderer, char *path, Vector position,
                      Vector size);
