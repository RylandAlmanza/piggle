#include <math.h>
#include "camera.h"
#include "rectangle.h"

void Camera_center_on(Camera *self, Entity *entity) {
    int half_width = floor(self->width / 2);
    int half_height = floor(self->height / 2);
    int entity_half_width = floor(entity->width / 2);
    int entity_half_height = floor(entity->height / 2);
    int entity_center_x = entity->x + entity_half_width;
    int entity_center_y = entity->y + entity_half_height;
    self->x = entity_center_x - half_width;
    self->y = entity_center_y - half_height;
    if (self->x < self->bounds.left) {
        self->x = self->bounds.left;
    }
    if (self->x + self->width > self->bounds.right) {
        self->x = self->bounds.right - self->width;
    }
    if (self->y < self->bounds.top) {
        self->y = self->bounds.top;
    }
    if (self->y + self->height > self->bounds.bottom) {
        self->y = self->bounds.bottom - self->height;
    }
}

bool Camera_is_visible(Camera *self, Entity *entity) {
    Rectangle rect = {
        .left = self->x,
        .top = self->y,
        .right = self->x + self->width,
        .bottom = self->y + self->height
    };
    return collides(rect, entity->get_hitbox(entity));
}

Camera Camera_new(int width, int height, Rectangle bounds) {
    Camera camera;
    camera.x = 0;
    camera.y = 0;
    camera.width = width;
    camera.height = height;
    camera.bounds = bounds;
    camera.center_on = &Camera_center_on;
    camera.is_visible = &Camera_is_visible;

    return camera;
}
