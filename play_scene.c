#include <stdbool.h>
#include "play_scene.h"

int play_scene_player_x;
int play_scene_player_y;
bool up_pressed;
bool down_pressed;

DrawActionList play_scene_update(EventList events) {
    DrawActionList actions = DrawActionList_new();
    int i;
    for (i = 0; i < events.length; i++) {
        Event event = events.events[i];
        if (event.type == KEYDOWN) {
            switch (event.value) {
                case KEY_UP:
                    up_pressed = true;
                    break;
                case KEY_DOWN:
                    down_pressed = true;
                    break;
            }
        }
        if (event.type == KEYUP) {
            switch (event.value) {
                case KEY_UP:
                    up_pressed = false;
                case KEY_DOWN:
                    down_pressed = false;
            }
        }
    }

    if (up_pressed) {
        play_scene_player_y--;
    }
    if (down_pressed) {
        play_scene_player_y++;
    }

    // Fill the surface black
    actions.add_action(&actions, 10, play_scene_player_y, "player");
    return actions;
}
