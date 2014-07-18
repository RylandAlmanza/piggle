#include "play_scene.h"

int play_scene_player_x;
int play_scene_player_y;

DrawActionList play_scene_update(EventList events) {
    DrawActionList actions = DrawActionList_new();
    int i;
    for (i = 0; i < events.length; i++) {
        Event event = events.events[i];
        if (event.type == KEYDOWN) {
            switch (event.value) {
                case KEY_UP:
                    play_scene_player_y--;
                    break;
                case KEY_DOWN:
                    play_scene_player_y++;
                    break;
            }
        }
    }

    // Fill the surface black
    actions.add_action(&actions, 10, play_scene_player_y, "player");
    return actions;
}
