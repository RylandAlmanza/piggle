#ifndef PLAY_SCENE_H
#define PLAY_SCENE_H

#include "draw_action.h"
#include "event.h"

void play_scene_start();
DrawActionList play_scene_update(EventList events);
void play_scene_end(EventList events);

#endif
