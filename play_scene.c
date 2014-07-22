#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "play_scene.h"
#include "world.h"
#include "camera.h"
#include "point.h"
#include "scene.h"

World play_world;
Camera play_camera;
bool play_up;
bool play_right;
bool play_down;
bool play_left;
bool play_action;
char play_log_text[255];
int play_choice;
bool play_choosing;

void play_log(char text[255]) {
    strcpy(play_log_text, text);
}

void play_choose_start() {
    play_choice = 0;
    play_choosing = true;
}

void play_scene_start() {
    play_choosing = false;
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
    play_camera = Camera_new(490, 450, bounds);
    play_world.add_entity(&play_world, "player", 96, 96);
    play_camera.center_on(&play_camera, player);
    play_log("Welcome to Piggle!");
}

void draw_log(DrawActionList *actions) {
    actions->add_action(actions, DrawAction_rectangle(0, 450, 490, 30, 222, 238,
                                                214));
    actions->add_action(actions, DrawAction_text(4, 455, play_log_text, 20, 12, 28));
}

void draw_inventory(DrawActionList *actions) {
    Entity *player = play_world.get_entity(&play_world, 0);
    InventoryItem *item;
    actions->add_action(actions, DrawAction_rectangle(490, 0, 150, 480,
                                                      222, 238, 214));
    if (play_choosing) {
        int y = (play_choice * 34);
        actions->add_action(actions, DrawAction_rectangle(490, y, 150, 34, 89,
                                                          125, 206));
    }
    int i;
    for (i = 0; i < player->item_count; i++) {
        item = &player->inventory[i];
        int y = 2 + (i * 34);
        actions->add_action(actions, DrawAction_sprite(492, y, item->sprite));
        actions->add_action(actions, DrawAction_text(526, y, item->name, 20, 12,
                                               28));
    }
}

void draw_world_view(DrawActionList *actions) {
    Entity *player = play_world.get_entity(&play_world, 0);
    play_camera.center_on(&play_camera, player);

    int i;
    for (i = 0; i < play_world.width * play_world.height; i++) {
        Entity *tile = &play_world.tiles[i];
        if (!play_camera.is_visible(&play_camera, tile)) {
            continue;
        }
        int x = tile->x - play_camera.x;
        int y = tile->y - play_camera.y;
        actions->add_action(actions, DrawAction_sprite(x, y, tile->sprite));
    }

    for (i = 1; i < play_world.entity_count; i++) {
        Entity *entity = play_world.get_entity(&play_world, i);
        
        int x = entity->x - play_camera.x;
        int y = entity->y - play_camera.y;

        actions->add_action(actions, DrawAction_sprite(x, y, entity->sprite));
    }

    int x = player->x - play_camera.x;
    int y = player->y - play_camera.y;

    actions->add_action(actions, DrawAction_sprite(x, y, player->sprite));
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

int sqr(int x) {
    return x * x;
}

Entity *get_facing_tile(Entity *entity) {
    Entity *tile1;
    Entity *tile2;
    Entity *tile3;
    Point p0;
    Point p1;
    Point p2;
    Point p3;
    int distance1;
    int distance2;
    int distance3;
    int x = floor(entity->x / 32);
    int y = floor(entity->y / 32);
    switch (entity->facing) {
        case NORTH:
            tile1 = play_world.get_tile(&play_world, x - 1, y - 1);
            tile2 = play_world.get_tile(&play_world, x, y - 1);
            tile3 = play_world.get_tile(&play_world, x + 1, y - 1);
            p0 = (Point){.x = entity->x, .y = entity->y};
            p1 = (Point){.x = tile1->x, .y = tile1->y + tile1->height};
            p2 = (Point){.x = tile2->x, .y = tile2->y + tile2->height};
            p3 = (Point){.x = tile3->x, .y = tile3->y + tile3->height};
            break;
        case EAST:
            tile1 = play_world.get_tile(&play_world, x + 1, y - 1);
            tile2 = play_world.get_tile(&play_world, x + 1, y);
            tile3 = play_world.get_tile(&play_world, x + 1, y + 1);
            p0 = (Point){.x = entity->x + entity->width, .y = entity->y};
            p1 = (Point){.x = tile1->x, .y = tile1->y};
            p2 = (Point){.x = tile2->x, .y = tile2->y};
            p3 = (Point){.x = tile3->x, .y = tile3->y};
            break;
        case SOUTH:
            tile1 = play_world.get_tile(&play_world, x - 1, y + 1);
            tile2 = play_world.get_tile(&play_world, x, y + 1);
            tile3 = play_world.get_tile(&play_world, x + 1, y + 1);
            p0 = (Point){.x = entity->x, .y = entity->y + entity->height};
            p1 = (Point){.x = tile1->x, .y = tile1->y};
            p2 = (Point){.x = tile2->x, .y = tile2->y};
            p3 = (Point){.x = tile3->x, .y = tile3->y};
            break;
        case WEST:
            tile1 = play_world.get_tile(&play_world, x - 1, y - 1);
            tile2 = play_world.get_tile(&play_world, x - 1, y);
            tile3 = play_world.get_tile(&play_world, x - 1, y + 1);
            p0 = (Point){.x = entity->x, .y = entity->y};
            p1 = (Point){.x = tile1->x + tile1->width, .y = tile1->y};
            p2 = (Point){.x = tile2->x + tile2->width, .y = tile2->y};
            p3 = (Point){.x = tile3->x + tile3->width, .y = tile3->y};
            break;
    }
    distance1 = sqrt(sqr(p1.x - p0.x) + sqr(p1.y - p0.y));
    distance2 = sqrt(sqr(p2.x - p0.x) + sqr(p2.y - p0.y));
    distance3 = sqrt(sqr(p3.x - p0.x) + sqr(p3.y - p0.y));

    int smallest;
    if (distance1 < distance2) {
        smallest = distance1;
    } else {
        smallest = distance2;
    }
    if (distance3 < smallest) {
        smallest = distance3;
    }

    if (smallest == distance1) {
        return tile1;
    } else if (smallest == distance2) {
        return tile2;
    } else if (smallest == distance3) {
        return tile3;
    }
}

void play_scene_choose(EventList *events, DrawActionList *actions) {
    Entity *player = play_world.get_entity(&play_world, 0);
    int i;
    for (i = 0; i < events->length; i++) {
        Event event = events->events[i];
        if (event.type == QUIT) {
            play_scene_end();
            return;
        }
        if (event.type == KEYDOWN) {
            switch (event.value) {
                case UP:
                    play_choice--;
                    if (play_choice < 0) {
                        play_choice = player->item_count - 1;
                    }
                    break;
                case DOWN:
                    play_choice++;
                    if (play_choice >= player->item_count) {
                        play_choice = 0;
                    }
                    break;
                case ACTION:
                    piggle_scene_over = true;
                    play_choosing = false;
                    int x = floor(player->x / 32) * 32;
                    int y = floor(player->y / 32) * 32;
                    Entity item = player->lose(player, play_choice);
                    play_world.add_entity(&play_world, item.name, x, y);
                    char log_message[255];
                    sprintf(log_message, "You drop the %s.", item.name);
                    play_log(log_message);
                    piggle_scene_next = &play_scene_update;
                    break;
            }
        }
    }

    draw_world_view(actions);
    draw_inventory(actions);
    draw_log(actions);
}

void play_scene_update(EventList *events, DrawActionList *actions) {
    play_action = false;
    int i;
    for (i = 0; i < events->length; i++) {
        Event event = events->events[i];
        if (event.type == QUIT) {
            play_scene_end();
            return;
        }
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
                case ACTION:
                    play_action = true;
                    break;
                case DROP:
                    piggle_scene_over = true;
                    play_choose_start();
                    piggle_scene_next = &play_scene_choose;
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
        player->facing = EAST;
        player->sprite = PLAYER_RIGHT;
    }
    if (play_left) {
        player->x_velocity = -player->speed;
        player->facing = WEST;
        player->sprite = PLAYER_LEFT;
    }
    if (play_up) {
        player->y_velocity = -player->speed;
        player->facing = NORTH;
        player->sprite = PLAYER_UP;
    }
    if (play_down) {
        player->y_velocity = player->speed;
        player->facing = SOUTH;
        player->sprite = PLAYER_DOWN;
    }
    if (play_action) {
        Entity *tile = get_facing_tile(player);
        if (strcmp(tile->name, "tree") == 0) {
            bool has_axe = false;
            int i;
            for (i = 0; i < player->item_count; i++) {
                if (strcmp(player->inventory[i].name, "axe") == 0) {
                    has_axe = true;
                }
            }
            if (has_axe) {
                if (player->acquire(player, Entity_factory("log"))) {
                    Entity new_tile = Entity_factory("grass");
                    new_tile.x = tile->x;
                    new_tile.y = tile->y;
                    *tile = new_tile;
                    play_log("You cut down the tree, and acquire a log");
                } else {
                    play_log("You don't have any room in your inventory");
                }
            }
        }
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

    draw_world_view(actions);
    draw_inventory(actions);
    draw_log(actions);
}

void play_scene_end() {
    play_world.destroy(&play_world);
}
