#include "entity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Entity *create_entity(SDL_Renderer *renderer, char *path, Vector position,
                      Vector size) {
    // Use this instead to load textures
    SDL_Texture *apple_texture = IMG_LoadTexture(renderer, path);

    return NULL;
}

// Maybe make a function called like render_entities
void render_entity(SDL_Renderer *renderer, Entity *entity) {
    SDL_Rect rect = {entity->position.x, entity->position.y, entity->size.x,
                     entity->size.y};

    SDL_RenderCopy(renderer, entity->texture, NULL, &rect);
}
