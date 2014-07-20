#include <stdbool.h>
#include <string.h>
#include "play_scene.h"
#include "world.h"
#include "camera.h"

World play_world;
Camera play_camera;
bool play_up;
bool play_right;
bool play_down;
bool play_left;

void play_scene_start() {
    play_world = World_load();
    Entity *player = play_world.get_entity(&play_world, 0);
    int world_pixel_width = play_world.width * 32;
    int world_pixel_height = play_world.height * 32;
    Rectangle bounds = {
        .left = 0,
        .top = 0,
        .right = world_pixel_width,
        .bottom = world_pixel_height
    };
    play_camera = Camera_new(640, 480, bounds);
    play_world.add_entity(&play_world, "player", 96, 96);
    play_camera.center_on(&play_camera, player);
}

void handle_collision(Entity *entity, int x_velocity, int y_velocity) {
    int i;
    for (i = 0; i < play_world.width * play_world.height; i++) {
        Entity *tile = &play_world.tiles[i];
        if (!tile->solid) {
            continue;
        }
        if (collides(entity->get_hitbox(entity), tile->get_hitbox(tile))) {
            if (x_velocity > 0) {
                entity->x = tile->x - entity->width;
            }
            if (x_velocity < 0) {
                entity->x = tile->x + tile->width;
            }
            if (y_velocity > 0) {
                entity->y = tile->y - entity->height;
            }
            if (y_velocity < 0) {
                entity->y = tile->y + tile->height;
            }
        }
    }
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
    handle_collision(player, player->x_velocity, 0);
    player->y += player->y_velocity;
    handle_collision(player, 0, player->y_velocity);

    play_camera.center_on(&play_camera, player);

    for (i = 0; i < play_world.width * play_world.height; i++) {
        Entity *tile = &play_world.tiles[i];
        if (!play_camera.is_visible(&play_camera, tile)) {
            continue;
        }
        int x = tile->x - play_camera.x;
        int y = tile->y - play_camera.y;
        actions.add_action(&actions, x, y, tile->sprite);
    }
    int x = player->x - play_camera.x;
    int y = player->y - play_camera.y;
    actions.add_action(&actions, x, y, player->sprite);
    return actions;
}
