#include <stdbool.h>
#include "play_scene.h"

int play_scene_player_x;
int play_scene_player_y;
bool up_pressed;
bool down_pressed;
bool right_pressed;
bool left_pressed;

DrawActionList play_scene_update(EventList events) {
    DrawActionList actions = DrawActionList_new();
    int i;
    for (i = 0; i < events.length; i++) {
        Event event = events.events[i];
        if (event.type == KEYDOWN) {
            switch (event.value) {
                case UP:
                    up_pressed = true;
                    break;
                case DOWN:
                    down_pressed = true;
                    break;
                case RIGHT:
                    right_pressed = true;
                    break;
                case LEFT:
                    left_pressed = true;
                    break;
            }
        }
        if (event.type == KEYUP) {
            switch (event.value) {
                case UP:
                    up_pressed = false;
                    break;
                case DOWN:
                    down_pressed = false;
                    break;
                case LEFT:
                    left_pressed = false;
                    break;
                case RIGHT:
                    right_pressed = false;
                    break;
            }
        }
    }

    if (up_pressed) {
        play_scene_player_y--;
    }
    if (down_pressed) {
        play_scene_player_y++;
    }
    if (right_pressed) {
        play_scene_player_x++;
    }
    if (left_pressed) {
        play_scene_player_x--;
    }

    // Fill the surface black
    actions.add_action(&actions, play_scene_player_x, play_scene_player_y,
                       "player");
    return actions;
}
