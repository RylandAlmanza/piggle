#ifndef START_SCENE_H
#define START_SCENE_H

#include "draw_action.h"
#include "event.h"

void start_scene_update(EventList *events, DrawActionList *actions);
void start_scene_end();

#endif
