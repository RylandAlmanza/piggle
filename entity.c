#include <string.h>
#include <stdlib.h>
#include "entity.h"
#include "sprites.h"

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
    self->inventory[self->item_count - 1].sprite = item.sprite;
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
    if (strcmp(name, "player") == 0) {
        entity.sprite = PLAYER_UP;
        entity.solid = true;
        entity.speed = 3;
        entity.acquire(&entity, Entity_factory("tinderbox"));
        entity.acquire(&entity, Entity_factory("axe"));
    } else if (strcmp(name, "water") == 0) {
        entity.sprite = WATER;
        entity.solid = true;
    } else if (strcmp(name, "tree") == 0) {
        entity.sprite = TREE;
        entity.solid = true;
    } else if (strcmp(name, "grass") == 0) {
        entity.sprite = GRASS;
    } else if (strcmp(name, "wheat") == 0) {
        entity.sprite = WHEAT;
    } else if (strcmp(name, "axe") == 0) {
        entity.sprite = AXE;
    } else if (strcmp(name, "tinderbox") == 0) {
        entity.sprite = TINDERBOX;
    } else if (strcmp(name, "log") == 0) {
        entity.sprite = LOG;
    }

    return entity;
}
