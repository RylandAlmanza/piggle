#include <stdio.h>
#include <stdbool.h>
#include "scene.h"
#include "start_scene.h"
#include "play_scene.h"

DrawActionList start_scene_update(EventList events) {
    DrawActionList actions = DrawActionList_new();
    int i;
    for (i = 0; i < events.length; i++) {
        Event event = events.events[i];
        if (event.type == KEYDOWN) {
            switch (event.value) {
                case UP:
                    //scene_over is never set back to false. Fix that shit.
                    piggle_scene_next = play_scene_update;
                    piggle_scene_over = true;
                    break;
            }
        }
    }

    actions.add_action(&actions, 10, 10, "start");
    return actions;
}
