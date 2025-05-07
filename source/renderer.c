#include "renderer.h"

#include <gl2d.h>

void renderPolygon(Triangle *triangle, matrix trans, Color *color) {
    vector origin = {0, 0, 1};
    vector trans_o = vec_transform(trans, origin);
    vector trans_a = vec_transform(trans, triangle->a);
    vector trans_b = vec_transform(trans, triangle->b);
 
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
