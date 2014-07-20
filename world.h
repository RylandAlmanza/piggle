#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

typedef struct WorldStruct World;

struct WorldStruct {
    int entity_count;
    int width;
    int height;
    Entity *tiles;
    Entity *entities;
    Entity *(*add_entity)(World *self, char *name, int x, int y);
    Entity *(*get_entity)(World *self, int entity_id);
    Entity (*remove_entity)(World *self, int entity_id);
    Entity *(*get_tile)(World *self, int x, int y);
    void (*destroy)(World *self);
};

World World_load();

#endif
