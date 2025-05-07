#include "polygon.h"

vector polygonCenter(Triangle *poly) {
  vector sum = vec_add(poly->a, poly->b);

  return vec_mul(sum, 0.5);
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
  poly->a = vec_transform(trans, poly->a);
  poly->b = vec_transform(trans, poly->b);
}


