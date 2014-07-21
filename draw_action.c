#include <string.h>
#include <stdlib.h>
#include "draw_action.h"

char pleb[255];

void DrawActionList_add_action(DrawActionList *self, DrawAction action) {
    self->length++;
    self->actions = realloc(self->actions,
                            sizeof(DrawAction) * self->length);
    self->actions[self->length - 1] = action;
}

DrawAction DrawAction_sprite(int x, int y, char sprite[255]) {
    DrawAction action;
    action.type = SPRITE;
    action.x = x;
    action.y = y;
    strcpy(pleb, sprite);
    strcpy(action.sprite, sprite);

    return action;
}

DrawAction DrawAction_rectangle(int x, int y, int width, int height, int red,
                                int green, int blue) {
    DrawAction action;
    action.type = RECTANGLE;
    action.x = x;
    action.y = y;
    action.width = width;
    action.height = height;
    action.red = red;
    action.green = green;
    action.blue = blue;

    return action;
}

DrawAction DrawAction_text(int x, int y, char text[255], int red, int green,
                           int blue) {
    DrawAction action;
    action.type = TEXT;
    strcpy(action.text, text);
    action.x = x;
    action.y = y;
    action.red = red;
    action.green = green;
    action.blue = blue;

    return action;
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
