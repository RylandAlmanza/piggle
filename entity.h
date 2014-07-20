#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#include "rectangle.h"

typedef struct EntityStruct Entity;

struct EntityStruct {
    int id;
    int x;
    int y;
    int width;
    int height;
    int speed;
    int x_velocity;
    int y_velocity;
    bool solid;
    char name[255];
    char sprite[255];
    Rectangle (*get_hitbox)(Entity *self);
};

Entity Entity_factory(char *name);

#endif
