#include <SDL2/SDL_render.h>
typedef struct {
    int x;
    int y;
} Vector;

typedef struct {
    Vector position;
    Vector size;
    double angle;
} Transform;

typedef struct {
    Transform transform;
    SDL_Texture *texture;
} Entity;

Entity *create_entity(SDL_Renderer *renderer, char *path, Vector position,
                      Vector size);

void render_entity(SDL_Renderer *renderer, Entity *entity);
