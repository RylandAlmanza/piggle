#include <string.h>
#include <stdlib.h>
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

bool Entity_acquire(Entity *self, Entity item) {
    if (self->item_count >= 25) {
        return false;
    }
    self->item_count++;
    strcpy(self->inventory[self->item_count - 1].name, item.name);
    strcpy(self->inventory[self->item_count - 1].sprite, item.sprite);
    return true;
}

Entity Entity_factory(char *name) {
    Entity entity;
    entity.id = 0;
    entity.destroyed = false;
    entity.x = 0;
    entity.y = 0;
    entity.width = 32;
    entity.height = 32;
    entity.speed = 0;
    entity.x_velocity = 0;
    entity.y_velocity = 0;
    entity.item_count = 0;
    entity.facing = NORTH;
    entity.solid = false;
    strcpy(entity.name, name);
    entity.get_hitbox = &Entity_get_hitbox;
    entity.acquire = &Entity_acquire;
    strcpy(entity.sprite, name);
    if (strcmp(name, "player") == 0) {
        strcpy(entity.sprite, "player_up");
        entity.solid = true;
        entity.speed = 3;
        entity.acquire(&entity, Entity_factory("tinderbox"));
        entity.acquire(&entity, Entity_factory("axe"));
    } else if (strcmp(name, "water") == 0) {
        entity.solid = true;
    } else if (strcmp(name, "tree") == 0) {
        entity.solid = true;
    }

    return entity;
}
