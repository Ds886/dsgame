#include "renderer.h"

#include <gl2d.h>

void renderPolygonTransformed(Polygon *poly, vector pos,  matrix trans, Color color) {
    Polygon trans_poly = transform(poly, trans);
    renderPolygon(&trans_poly, pos, color);
}

void renderPolygon(Polygon *poly, vector pos, Color color) {
    vector first_vertex = make_vec(0,0,0);
    vector prev_vertex = make_vec(0,0,0);
    for (int i = 0; i < poly->num_vertices; i++) {
        vector trans_vertex = vec_add(VERTEX(poly, i), pos);
        
        if (i > 0) {
            glLine(
                X(trans_vertex),
                Y(trans_vertex),
                X(prev_vertex),
                Y(prev_vertex),
                COLOR_TO_15BIT(color)
            );
        } else {
            first_vertex = trans_vertex;
        }

        prev_vertex = trans_vertex;
    }

    glLine(
        X(first_vertex),
        Y(first_vertex),
        X(prev_vertex),
        Y(prev_vertex),
        COLOR_TO_15BIT(color)
    );
}
