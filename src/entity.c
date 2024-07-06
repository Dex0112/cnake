#include "entity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
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

    Entity *entity = (Entity *)malloc(sizeof(Entity));
    entity->transform.position = position;
    entity->transform.size = size;
    entity->texture = texture;
    entity->transform.angle = 0;

    return entity;
}

// Maybe make a function called like render_entities
void render_entity(SDL_Renderer *renderer, Entity *entity) {
    SDL_Rect rect = {entity->transform.position.x, entity->transform.position.y,
                     entity->transform.size.x, entity->transform.size.y};

    // Render at the center of the texture
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;

    SDL_Point rotation_center = {rect.w / 2, rect.h / 2};
    SDL_RenderCopyEx(renderer, entity->texture, NULL, &rect, entity->transform.angle, &rotation_center, SDL_FLIP_NONE);
}
