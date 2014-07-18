#ifndef START_SCENE_H
#define START_SCENE_H

#include "draw_action.h"
#include "event.h"

DrawActionList start_scene_update(EventList events);
void start_scene_end(EventList events);

#endif
