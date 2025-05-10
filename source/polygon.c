#include "polygon.h"



#include <nds.h>

vector polygonCenter(Polygon *poly) {
  vector sum = make_vec(0,0,0); 

  for (int i=0;i < poly->num_vertices;i++) {
    sum = vec_add(sum, VERTEX(poly, i));
  }
  
  return vec_mul(sum, 1.0/poly->num_vertices);
}

void polygonMove(Polygon *poly, vector v) {
  for (int i=0; i < poly->num_vertices; i++) {
    VERTEX(poly, i) = vec_add(VERTEX(poly, i), v);
  }
}

Polygon isoscelesTriangleCentered(float base, float height) {
  Polygon res = isoscelesTriangle(base, height, make_vec(0,0,0));
  vector center = vec_mul(polygonCenter(&res), -1);
  polygonMove(&res, center);

  return res;
}

Polygon regularPolygon(int num_vertices, float size) {
  Polygon res;

  res.num_vertices = num_vertices;

  for (int i = 0; i < num_vertices; i++) {
    float rot = ((float)i * 360) / num_vertices;
    s16 bin_rotation = degreesToAngle(rot);
    float cos = fixedToFloat(cosLerp(bin_rotation), 12);
    float sin = fixedToFloat(sinLerp(bin_rotation), 12);
    VERTEX(&res, i) =  MAKE_VEC2(size * cos, size * sin);
  }

  return res;
}

Polygon isoscelesTriangle(float base, float height, vector center) {
  Polygon res;
  vector *a, *b, *c;

  a = &VERTEX(&res, 0);
  b = &VERTEX(&res, 1);
  c = &VERTEX(&res, 2);
    
  *a = make_vec(-base/2, height, 0);
  *b = make_vec(base/2, height, 0);
  *c = make_vec(0, 0, 0);

  *a = vec_add(*a, center);
  *b = vec_add(*b, center);
  *c = vec_add(*c, center);

  res.num_vertices = 3;
  return res;
}

void transform(Polygon *poly, matrix trans) { 
  for (int i=0; i < poly->num_vertices; i++) {
    VERTEX(poly, i) = vec_transform(trans, VERTEX(poly, i));
  }
}


