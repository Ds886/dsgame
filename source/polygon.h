#ifndef POLYGON_H
#define POLYGON_H 1
// your code
#include "linalg.h"

#define POLYGON_MAX_VERTICES 20

#define VERTEX(p, i) ((p)->v[i])

typedef struct triangle_t {
  vector a;
  vector b;
  vector c;
} Triangle;

typedef struct polygon_t {
  vector v[POLYGON_MAX_VERTICES];
  int num_vertices;
} Polygon;

vector polygonCenter(Polygon *poly);

Polygon isoscelesTriangle(float base, float height, vector center);

Polygon isoscelesTriangleCentered(float base, float height);

Polygon regularPolygon(int num_vertices, float size);

Polygon almostRegularPolygon(int num_vertices, float size, float deviation);

Polygon rectangle(vec2 a, vec2 b);

//TODO: currntly there's no distinction between box and rectangle 
Polygon boundingBox(Polygon *poly);

void transform(Polygon *poly, matrix trans);
#endif
