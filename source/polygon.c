#include "polygon.h"

vector polygonCenter(Triangle *poly) {
  vector sum = vec_add(poly->a, poly->b);

  return vec_mul(sum, 0.5);
}

Triangle isoscelesTriangle(float base, float height) {
  Triangle res;
  res.a = make_vec(-base/2, height, 1);
  res.b = make_vec(base/2, height, 1);

  return res;
}

void transform(Triangle *poly, matrix trans) { 
  poly->a = vec_transform(trans, poly->a);
  poly->b = vec_transform(trans, poly->b);
}


