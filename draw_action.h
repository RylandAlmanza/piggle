#ifndef DRAW_ACTION_H
#define DRAW_ACTION_H

#include "sprites.h"

typedef enum ActionTypeEnum ActionType;

enum ActionTypeEnum {
    SPRITE,
    RECTANGLE,
    TEXT
};

typedef struct DrawActionRectStruct DrawActionRect;

struct DrawActionRectStruct {
    int red;
    int green;
    int blue;
    int width;
    int height;
};

typedef struct DrawActionTextStruct DrawActionText;

struct DrawActionTextStruct {
    char text[255];
    int red;
    int green;
    int blue;
};

typedef struct DrawActionStruct DrawAction;

struct DrawActionStruct {
    ActionType type;
    int x;
    int y;
    union {
        int sprite;
        DrawActionText text;
        DrawActionRect rect;
    };
};

typedef struct DrawActionListStruct DrawActionList;

struct DrawActionListStruct {
    DrawAction *actions;
    int length;
    void (*add_action)(DrawActionList *self, DrawAction action);
    void (*destroy)(DrawActionList *self);
};

DrawAction DrawAction_sprite(int x, int y, int sprite);
DrawAction DrawAction_rectangle(int x, int y, int width, int height, int red,
                                int green, int blue);
DrawAction DrawAction_text(int x, int y, char text[255], int red, int green,
                           int blue);

DrawActionList DrawActionList_new();

#endif
