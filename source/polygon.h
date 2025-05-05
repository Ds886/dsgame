#ifndef POLYGON_H
#define POLYGON_H 1
// your code
#include "linalg.h"

typedef struct triangle_t {
  struct vec3 a;
  struct vec3 b;
} Triangle;


struct vec3 polygonCenter(Triangle *poly);

Triangle isoscelesTriangle(float base, float height);

void transform(Triangle *poly, matrix trans);
#endif
