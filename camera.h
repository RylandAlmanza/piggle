#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include "entity.h"

typedef struct CameraStruct Camera;

struct CameraStruct {
    int x;
    int y;
    int width;
    int height;
    Rectangle bounds;
    void (*center_on)(Camera *self, Entity *entity);
    bool (*is_visible)(Camera *self, Entity *entity);
};

Camera Camera_new(int width, int height, Rectangle bounds);

#endif
