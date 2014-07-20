#include <stdbool.h>
#include <string.h>
#include "play_scene.h"
#include "world.h"

World play_world;
bool play_up;
bool play_right;
bool play_down;
bool play_left;

void play_scene_start() {
    play_world = World_load();
    play_world.add_entity(&play_world, "player", 10, 10);
}

DrawActionList play_scene_update(EventList events) {
    DrawActionList actions = DrawActionList_new();
    int i;
    for (i = 0; i < events.length; i++) {
        Event event = events.events[i];
        if (event.type == KEYDOWN) {
            switch (event.value) {
                case UP:
                    play_up = true;
                    break;
                case DOWN:
                    play_down = true;
                    break;
                case RIGHT:
                    play_right = true;
                    break;
                case LEFT:
                    play_left = true;
                    break;
            }
        }
        if (event.type == KEYUP) {
            switch (event.value) {
                case UP:
                    play_up = false;
                    break;
                case DOWN:
                    play_down = false;
                    break;
                case LEFT:
                    play_left = false;
                    break;
                case RIGHT:
                    play_right = false;
                    break;
            }
        }
    }
    Entity *player = play_world.get_entity(&play_world, 0);

    if (play_right) {
        player->x_velocity = player->speed;
        strcpy(player->sprite, "player_right");
    }
    if (play_left) {
        player->x_velocity = -player->speed;
        strcpy(player->sprite, "player_left");
    }
    if (play_up) {
        player->y_velocity = -player->speed;
        strcpy(player->sprite, "player_up");
    }
    if (play_down) {
        player->y_velocity = player->speed;
        strcpy(player->sprite, "player_down");
    }
    if (!(play_up || play_down)) {
        player->y_velocity = 0;
    }
    if (!(play_right || play_left)) {
        player->x_velocity = 0;
    }

    player->x += player->x_velocity;
    for (i = 0; i < play_world.width * play_world.height; i++) {
        Entity *tile = &play_world.tiles[i];
        if (!tile->solid) {
            continue;
        }
        if (collides(player->get_hitbox(player), tile->get_hitbox(tile))) {
            if (player->x_velocity > 0) {
                player->x = tile->x - player->width;
            }
            if (player->x_velocity < 0) {
                player->x = tile->x + tile->width;
            }
        }
    }

    player->y += player->y_velocity;
    for (i = 0; i < play_world.width * play_world.height; i++) {
        Entity *tile = &play_world.tiles[i];
        if (!tile->solid) {
            continue;
        }
        if (collides(player->get_hitbox(player), tile->get_hitbox(tile))) {
            if (player->y_velocity > 0) {
                player->y = tile->y - player->height;
            }
            if (player->y_velocity < 0) {
                player->y = tile->y + tile->height;
            }
        }
    }

    for (i = 0; i < play_world.width * play_world.height; i++) {
        Entity *tile = &play_world.tiles[i];
        actions.add_action(&actions, tile->x, tile->y, tile->sprite);
    }

    actions.add_action(&actions, player->x, player->y, player->sprite);
    return actions;
}
