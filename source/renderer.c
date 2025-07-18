#include "renderer.h"

#include <nds/arm9/videoGL.h>

void renderPolygonTransformed(Polygon *poly, vector pos,  matrix trans, Color color) {
    Polygon trans_poly = transform(poly, trans);
    renderPolygon(&trans_poly, pos, color);
}

void renderPolygon(Polygon *poly, vector pos, Color color) {
    glBegin(GL_TRIANGLE_STRIP);
    
    glColor(COLOR_TO_15BIT(color));

    glPolyFmt(
      POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0)
    );


    for (int i = 0; i < poly->num_vertices; i++) {
        vector vertex = vec_add(VERTEX(poly, i), pos);
        
        glVertex3v16(
            X(vertex),
            Y(vertex),
            0
        );
    }

    // TODO: this is a workaround for drawing lines.
    // a better method is required.
    if (poly->num_vertices == 2) {
        vector vertex = vec_add(VERTEX(poly, 1), pos);
        vertex = vec_add(vertex, MAKE_VEC2(1, 0));
        
        glVertex3v16(
            X(vertex),
            Y(vertex),
            0
        );
    }

    glEnd();
}
