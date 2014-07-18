#ifndef DRAW_ACTION_H
#define DRAW_ACTION_H

typedef struct DrawActionStruct DrawAction;

struct DrawActionStruct {
    char sprite[255];
    int x;
    int y;
};

typedef struct DrawActionListStruct DrawActionList;

struct DrawActionListStruct {
    DrawAction *actions;
    int length;
    void (*add_action)(DrawActionList *self, int x, int y, char sprite[255]);
    void (*destroy)(DrawActionList *self);
};

DrawActionList DrawActionList_new();

#endif
