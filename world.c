#include <stddef.h>
#include <stdlib.h>
#include "world.h"

int TILE_WIDTH = 32;
int TILE_HEIGHT = 32;

Entity *World_add_entity(World *self, char *name, int x, int y) {
    self->entity_count++;
    self->entities = realloc(self->entities,
                             sizeof(Entity) * self->entity_count);
    Entity entity = Entity_factory(name);
    entity.x = x;
    entity.y = y;
    entity.id = self->entity_count - 1;
    self->entities[entity.id] = entity;
    return &self->entities[entity.id];
}

Entity *World_get_entity(World *self, int entity_id) {
    return &self->entities[entity_id];
}

Entity *World_get_tile(World *self, int x, int y) {
    return &self->tiles[(y * self->width) + x];
}

Entity World_remove_entity(World *self, int entity_id) {
    Entity entity = self->entities[entity_id];
    // An ID of -1 implies that this entity should be ignored
    self->entities[entity_id].id = -1;
    return entity;
}

void World_destroy(World *self) {
    free(self->entities);
    free(self->tiles);
}

World World_load() {
    World world;
    world.entity_count = 0;
    world.width = 5;
    world.height = 5;
    world.tiles = malloc(sizeof(Entity) * (world.width * world.height));
    int x, y;
    for (y = 0; y < world.height; y++) {
        for (x = 0; x < world.width; x++) {
            Entity tile;
            if (rand() % 2) {
                tile = Entity_factory("grass");
            } else {
                tile = Entity_factory("water");
            }
            tile.x = x * TILE_WIDTH;
            tile.y = y * TILE_HEIGHT;
            world.tiles[(y * world.width) + x] = tile;
        }
    }
    world.entities = malloc(sizeof(Entity));

    world.add_entity = &World_add_entity;
    world.get_entity = &World_get_entity;
    world.remove_entity = &World_remove_entity;
    world.get_tile = &World_get_tile;
    world.destroy = &World_destroy;
    
    return world;
}