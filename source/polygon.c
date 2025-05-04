#include "polygon.h"

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


