#include <string.h>
#include <stdlib.h>
#include "draw_action.h"

void DrawActionList_add_action(DrawActionList *self, DrawAction action) {
    self->length++;
    self->actions = realloc(self->actions,
                            sizeof(DrawAction) * self->length);
    self->actions[self->length - 1] = action;
}

DrawAction DrawAction_sprite(int x, int y, int sprite) {
    DrawAction action;
    action.type = SPRITE;
    action.x = x;
    action.y = y;
    action.sprite = sprite;

    return action;
}

DrawAction DrawAction_rectangle(int x, int y, int width, int height, int red,
                                int green, int blue) {
    DrawAction action;
    action.type = RECTANGLE;
    action.x = x;
    action.y = y;
    action.rect.width = width;
    action.rect.height = height;
    action.rect.red = red;
    action.rect.green = green;
    action.rect.blue = blue;

    return action;
}

DrawAction DrawAction_text(int x, int y, char text[255], int red, int green,
                           int blue) {
    DrawAction action;
    action.type = TEXT;
    strcpy(action.text.text, text);
    action.x = x;
    action.y = y;
    action.text.red = red;
    action.text.green = green;
    action.text.blue = blue;

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
