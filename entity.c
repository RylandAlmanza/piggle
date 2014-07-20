#include <string.h>
#include "entity.h"

Rectangle Entity_get_hitbox(Entity *self) {
    Rectangle hitbox = {
        .left = self->x,
        .top = self->y,
        .right = self->x + self->width,
        .bottom = self->y + self->height
    };
    return hitbox;
}

Entity Entity_factory(char *name) {
    Entity entity;
    entity.id = 0;
    entity.x = 0;
    entity.y = 0;
    entity.width = 32;
    entity.height = 32;
    entity.speed = 0;
    entity.x_velocity = 0;
    entity.y_velocity = 0;
    entity.solid = false;
    strcpy(entity.name, name);
    if (strcmp(name, "player") == 0) {
        strcpy(entity.sprite, "player_up");
        entity.solid = true;
        entity.speed = 3;
    } else if (strcmp(name, "grass") == 0) {
        strcpy(entity.sprite, "grass");
    } else if (strcmp(name, "water") == 0) {
        strcpy(entity.sprite, "water");
        entity.solid = true;
    }

    entity.get_hitbox = &Entity_get_hitbox;
    return entity;
}
