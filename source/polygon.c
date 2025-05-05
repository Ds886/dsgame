#include "polygon.h"

struct vec3 polygonCenter(Triangle *poly) {
  struct vec3 sum = vec3_add(poly->a, poly->b);

  return vec3_mul(sum, 0.5);
}

Triangle isoscelesTriangle(float base, float height) {
  Triangle res;
  res.a.x = -base / 2;
  res.a.y = height;
  res.a.z = 1;

  res.b.x = base / 2;
  res.b.y = height;
  res.b.z = 1;

  return res;
}

void transform(Triangle *poly, matrix trans) {
  poly->a = vec3_transform(trans, poly->a);
  poly->b = vec3_transform(trans, poly->b);
}


