#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#include "rectangle.h"
#include "direction.h"
#include "inventory_item.h"

typedef struct EntityStruct Entity;

struct EntityStruct {
    int id;
    bool destroyed;
    int x;
    int y;
    int width;
    int height;
    int speed;
    int x_velocity;
    int y_velocity;
    InventoryItem inventory[25];
    int item_count;
    Direction facing;
    bool solid;
    char name[255];
    int sprite;
    Rectangle (*get_hitbox)(Entity *self);
    bool (*acquire)(Entity *self, Entity item);
    Entity (*lose)(Entity *self, int index);
};

Entity Entity_factory(char *name);

#endif
