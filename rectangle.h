#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdbool.h>

typedef struct RectangleStruct Rectangle;

struct RectangleStruct {
    int left;
    int top;
    int right;
    int bottom;
};

bool collides(Rectangle a, Rectangle b);

#endif
