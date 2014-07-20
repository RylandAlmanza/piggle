#include "rectangle.h"

bool collides(Rectangle a, Rectangle b) {
    return (a.left < b.right && a.right > b.left &&
            a.top < b.bottom && a.bottom > b.top);
}

int get_width(Rectangle rect) {
    return rect.right - rect.left;
}

int get_height(Rectangle rect) {
    return rect.bottom - rect.top;
}
