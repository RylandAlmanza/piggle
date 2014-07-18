#include <string.h>
#include <stdlib.h>
#include "draw_action.h"

void DrawActionList_add_action(DrawActionList *self, int x, int y,
                               char sprite[255]) {
    DrawAction action;
    action.x = x;
    action.y = y;
    strcpy(action.sprite, sprite);
    
    self->length++;
    self->actions = realloc(self->actions,
                            sizeof(DrawAction) * self->length);
    self->actions[self->length - 1] = action;
}

void DrawActionList_destroy(DrawActionList *self) {
    free(self->actions);
}

DrawActionList DrawActionList_new() {
    DrawActionList list;
    list.length = 0;
    list.actions = malloc(sizeof(DrawAction));
    list.add_action = DrawActionList_add_action;
    list.destroy = DrawActionList_destroy;

    return list;
}
