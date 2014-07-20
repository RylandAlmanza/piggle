#ifndef DRAW_ACTION_H
#define DRAW_ACTION_H

typedef enum ActionTypeEnum ActionType;

enum ActionTypeEnum {
    SPRITE,
    RECTANGLE,
    TEXT
};

typedef struct DrawActionStruct DrawAction;

struct DrawActionStruct {
    ActionType type;
    char sprite[255];
    char text[255];
    int red;
    int green;
    int blue;
    int x;
    int y;
    int width;
    int height;
};

typedef struct DrawActionListStruct DrawActionList;

struct DrawActionListStruct {
    DrawAction *actions;
    int length;
    void (*add_action)(DrawActionList *self, DrawAction action);
    void (*destroy)(DrawActionList *self);
};

DrawAction DrawAction_sprite(int x, int y, char sprite[255]);
DrawAction DrawAction_rectangle(int x, int y, int width, int height, int red,
                                int green, int blue);
DrawAction DrawAction_text(int x, int y, char text[255], int red, int green,
                           int blue);

DrawActionList DrawActionList_new();

#endif
