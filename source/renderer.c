#include "renderer.h"

#include <gl2d.h>

void renderPolygon(Triangle *triangle, matrix trans, Color *color) {
    vector origin = make_vec(0, 0, 1);
    vector trans_o = vec_transform(trans, origin);
    vector trans_a = vec_transform(trans, triangle->a);
    vector trans_b = vec_transform(trans, triangle->b);
 
    glTriangleFilled(
        X(trans_o),
        Y(trans_o),
        X(trans_a),
        Y(trans_a),
        X(trans_b),
        Y(trans_b),
        COLOR_TO_15BIT(color)
    );
}
