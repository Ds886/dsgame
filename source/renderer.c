#include "renderer.h"

#include <gl2d.h>

void renderPolygon(Triangle *triangle, vector pos,  matrix trans, Color *color) {
    vector origin = ZERO_VEC;
    vector trans_a = vec_transform(trans, triangle->a);
    vector trans_b = vec_transform(trans, triangle->b);
    vector trans_c = vec_transform(trans, triangle->c);
    
    origin = vec_add(origin, pos);
    trans_a = vec_add(trans_a, pos);
    trans_b = vec_add(trans_b, pos);
    trans_c = vec_add(trans_c, pos);
 
    glTriangleFilled(
        X(trans_a),
        Y(trans_a),
        X(trans_b),
        Y(trans_b),
        X(trans_c),
        Y(trans_c),
        COLOR_TO_15BIT(color)
    );
}
