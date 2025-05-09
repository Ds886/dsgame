#include "polygon.h"

vector polygonCenter(Triangle *poly) {
  vector sum = vec_add(vec_add(poly->a, poly->b), poly->c);

  return vec_mul(sum, 1.0/3);
}

void polygonMove(Triangle *poly, vector v) {
  poly->a = vec_add(poly->a, v);
  poly->b = vec_add(poly->b, v);
  poly->c = vec_add(poly->c, v);
}

Triangle isoscelesTriangleCentered(float base, float height) {
  Triangle res = isoscelesTriangle(base, height, make_vec(0,0,0));
  vector center = vec_mul(polygonCenter(&res), -1);
  polygonMove(&res, center);

  return res;
}

Triangle isoscelesTriangle(float base, float height, vector center) {
  Triangle res;
  res.a = make_vec(-base/2, height, 0);
  res.b = make_vec(base/2, height, 0);
  res.c = make_vec(0, 0, 0);

  res.a = vec_add(res.a, center);
  res.b = vec_add(res.b, center);
  res.c = vec_add(res.c, center);

  return res;
}

void transform(Triangle *poly, matrix trans) { 
  poly->a = vec_transform(trans, poly->a);
  poly->b = vec_transform(trans, poly->b);
  poly->c = vec_transform(trans, poly->c);
}


