#include "renderer.h"

#include <gl2d.h>

void renderPolygon(Triangle *triangle, Color *color, struct vec2 pos, float rotation) {
    struct vec3 pos3;
    pos3.x = pos.x;
    pos3.y = pos.y;
    pos3.z = 0;
    struct vec3 a_t = vec3_add(triangle->a, pos3);
    struct vec3 b_t = vec3_add(triangle->b, pos3);
    glTriangleFilled(
        a_t.x,
        a_t.y,
        b_t.x,
        b_t.y,
        pos.x,
        pos.y,
        COLOR_TO_15BIT(color)
    );
}
