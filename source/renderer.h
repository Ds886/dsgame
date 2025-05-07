#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "polygon.h"
#include "linalg.h"

#define MAX_COLOR_BITS 5
#define FLOAT_TO_5BITS(n) ((uint16_t)(((1 << MAX_COLOR_BITS)-1)*n))
#define COLOR_TO_15BIT(col) (RGB15(FLOAT_TO_5BITS(COORD(col)->x), FLOAT_TO_5BITS(COORD(col)->y), FLOAT_TO_5BITS(COORD(col)->z)))

typedef vector Color;

void renderPolygon(Triangle *triangle, matrix m, Color *color);

#endif
