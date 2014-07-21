#ifndef SCENE_H
#define SCENE_H

#include "event.h"
#include "draw_action.h"

// This function pointer should point to the update function of whatever the
// current scene of the game is
void (*piggle_scene_update)(EventList *events,
                                      DrawActionList *actions);

// If a scene sets piggle_scene_over to true, the update function will be
// changed to whatever piggle_scene_next points to. (piggle_scene_next should
// obviously be set to point to the scene that you want to switch to.)
bool piggle_scene_over;
void (*piggle_scene_next)(EventList *events, DrawActionList *actions);

#endif
