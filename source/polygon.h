#ifndef POLYGON_H
#define POLYGON_H 1
// your code
#include "linalg.h"

typedef struct triangle_t {
  vector a;
  vector b;
  vector c;
} Triangle;


vector polygonCenter(Triangle *poly);

Triangle isoscelesTriangle(float base, float height, vector center);

Triangle isoscelesTriangleCentered(float base, float height);

void transform(Triangle *poly, matrix trans);
#endif
