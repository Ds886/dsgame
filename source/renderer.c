#include "renderer.h"

#include <gl2d.h>

void renderPolygon(Triangle *triangle, Color *color, struct vec2 pos, float rotation) {
    glTriangleFilled(
        triangle->a.x,
        triangle->a.y,
        triangle->b.x,
        triangle->b.y,
        triangle->c.x,
        triangle->c.y,
        COLOR_TO_15BIT(color)
    );
}
