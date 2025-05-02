#include "polygon.h"

Triangle isoscelesTriangle(float base, float height) {
  Triangle res;
  res.a.x = -base / 2;
  res.a.y = -height / 3;
  res.a.z = 0;

  res.b.x = base / 2;
  res.b.y = -height / 3;
  res.b.z = 0;

  res.c.x = 0;
  res.c.y = 2 * height / 3;
  res.c.z = 0;

  return res;
}

void transform(Triangle *poly, matrix trans) {
  poly->a = vec3_transform(trans, poly->a);
  poly->b = vec3_transform(trans, poly->b);
  poly->c = vec3_transform(trans, poly->c);
}


