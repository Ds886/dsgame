#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "polygon.h"
#include "linalg.h"

#define MAX_COLOR_BITS 5
#define FLOAT_TO_5BITS(n) ((uint16_t)(((1 << MAX_COLOR_BITS)-1)*n))
#define COLOR_TO_15BIT(col) (RGB15(FLOAT_TO_5BITS(VGET(col, 0)), FLOAT_TO_5BITS(VGET(col, 1)), FLOAT_TO_5BITS(VGET(col, 2))))

typedef vector Color;

//TODO: code dup! delete these!
#define GAME_SCREEN_WIDTH 256
#define GAME_SCREEN_HEIGHT 192

#define GLX(x) ((2*(float)(x) / (GAME_SCREEN_WIDTH)) - 1)
#define GLY(y) (1 - (2*(float)(y) / (GAME_SCREEN_HEIGHT)))

void renderPolygonTransformed(Polygon *poly, vector pos,  matrix m, Color color);
void renderPolygon(Polygon *poly, vector pos, Color color);

#endif
