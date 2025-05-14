#ifndef POLYGON_H
#define POLYGON_H 1

#include "linalg.h"

#include <nds.h>

#define POLYGON_MAX_VERTICES 20

#define VERTEX(p, i) ((p)->v[i])

#define BOUNDING_BOX_SIZE 0.7

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

Polygon transform(Polygon *poly, matrix trans);

bool checkCollision(Polygon *poly1, vector pos1, Polygon *poly2, vector pos2, Polygon *collision);
#endif
