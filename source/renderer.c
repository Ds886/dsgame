#include "renderer.h"

#include <gl2d.h>

void renderPolygon(Triangle *triangle, matrix trans, Color *color) {
    struct vec3 origin = {0, 0, 1};
    struct vec3 trans_o = vec3_transform(trans, origin);
    struct vec3 trans_a = vec3_transform(trans, triangle->a);
    struct vec3 trans_b = vec3_transform(trans, triangle->b);
 
    glTriangleFilled(
        trans_o.x,
        trans_o.y,
        trans_a.x,
        trans_a.y,
        trans_b.x,
        trans_b.y,
        COLOR_TO_15BIT(color)
    );
}
