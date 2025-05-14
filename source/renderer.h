#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "polygon.h"
#include "linalg.h"

#define MAX_COLOR_BITS 5
#define FLOAT_TO_5BITS(n) ((uint16_t)(((1 << MAX_COLOR_BITS)-1)*n))
#define COLOR_TO_15BIT(col) (RGB15(FLOAT_TO_5BITS(VGET(col, 0)), FLOAT_TO_5BITS(VGET(col, 1)), FLOAT_TO_5BITS(VGET(col, 2))))

#define MAKE_COLOR(x, y, z) make_vec(x, y, z)
#define RED      MAKE_COLOR(1, 0, 0)
#define GREEN    MAKE_COLOR(0, 1, 0)
#define BLUE     MAKE_COLOR(0, 0, 1)
#define YELLOW   MAKE_COLOR(1, 1, 0)
#define MAGENTA  MAKE_COLOR(0, 1, 1)
#define WHITE    MAKE_COLOR(1, 1, 1)
#define GREY     MAKE_COLOR(0.2, 0.2, 0.2)
#define BROWN    MAKE_COLOR(0.5, 0.1, 0)

typedef vector Color;

void renderPolygonTransformed(Polygon *poly, vector pos,  matrix m, Color color);
void renderPolygon(Polygon *poly, vector pos, Color color);

#endif
