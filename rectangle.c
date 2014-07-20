#include "rectangle.h"

bool collides(Rectangle a, Rectangle b) {
    return (a.left < b.right && a.right > b.left &&
            a.top < b.bottom && a.bottom > b.top);
}
