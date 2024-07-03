#include "entity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

Entity *create_entity(SDL_Renderer *renderer, char *path, Vector position,
                      Vector size) {
    // Use this instead to load textures
    printf("Loading Texture\n");
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL) {
        printf("Texture not loaded!");
    } 

    Entity *entity = (Entity *) malloc(sizeof(Entity));
    entity->position = position;
    entity->size = size;
    entity->texture = texture;

    return entity; 
}

// Maybe make a function called like render_entities
void render_entity(SDL_Renderer *renderer, Entity *entity) {
    SDL_Rect rect = {entity->position.x, entity->position.y, entity->size.x,
                     entity->size.y};

    SDL_RenderCopy(renderer, entity->texture, NULL, &rect);
}
