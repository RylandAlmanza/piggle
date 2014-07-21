#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int bagel;
void World_destroy(World *self) {
    free(self->entities);
    free(self->tiles);
}

World World_load() {
    World world;
    world.entities = malloc(sizeof(Entity));
    FILE *data;
    data = fopen("map.txt", "r");
    fscanf(data, "%d", &world.width);
    fscanf(data, "%d", &world.height);
    world.entity_count = 0;
    world.tiles = malloc(sizeof(Entity) * (world.width * world.height));
    int x, y;
    char line[world.width];
    for (y = 0; y < world.height; y++) {
        fscanf(data, "%s", line);
        for (x = 0; x < world.width; x++) {
            Entity tile;
            if (line[x] == '.') {
                tile = Entity_factory("grass");
            } else if (line[x] == '~') {
                tile = Entity_factory("water");
            } else if (line[x] == 'T') {
                tile = Entity_factory("tree");
            } else if (line[x] == '"') {
                tile = Entity_factory("wheat");
            }
            tile.x = x * TILE_WIDTH;
            tile.y = y * TILE_HEIGHT;
            world.tiles[(y * world.width) + x] = tile;
        }
    }
    fclose(data);

    world.get_entity = &World_get_entity;
    world.remove_entity = &World_remove_entity;
    world.get_tile = &World_get_tile;
    world.destroy = &World_destroy;
    world.add_entity = &World_add_entity;
    
    return world;
}
