#include "polygon.h"

vector polygonCenter(Polygon *poly) {
  vector sum = make_vec(0,0,0); 

  for (int i=0;i < poly->num_vertices;i++) {
    sum = vec_add(sum, VERTEX(poly, i));
  }
  
  return vec_mul(sum, 1.0/poly->num_vertices);
}

Polygon boundingBox(Polygon *poly) {
  float max[VEC_SIZE];
  float min[VEC_SIZE];

  for (int i = 0; i < VEC_SIZE; i++) {
    max[i] = VGET(VERTEX(poly, 0), i);
    min[i] = max[i];
  }

  for (int i=1; i< poly->num_vertices; i++) {
    vector *v = &VERTEX(poly, i);
    for (int j=0; j < VEC_SIZE; j++) {
      int c = VGET(*v, j);
      if (c > max[j])
        max[j] = c;
      if (c < min[j])
        min[j] =  c;    
    }
  }

  //TODO for now we ignore the elements after the 2nd
  Polygon rect = rectangle(MAKE_VEC2(min[0], min[1]), MAKE_VEC2(max[0], max[1]));
  matrix scaler_mat = mat_scaling(BOUNDING_BOX_SIZE);

  return transform(&rect, scaler_mat);
}

Polygon newLine(vector end) {
  Polygon l;

  l.num_vertices = 2;

  VERTEX(&l, 0) = ZERO_VEC;
  VERTEX(&l, 1) = end;

  return l;
}

void polygonMove(Polygon *poly, vector v) {
  for (int i=0; i < poly->num_vertices; i++) {
    VERTEX(poly, i) = vec_add(VERTEX(poly, i), v);
  }
}

Polygon rectangle(vec2 a, vec2 b) {
  Polygon rect;
  vector diff = vec_sub(b, a);

  VERTEX(&rect, 0) = a;
  VERTEX(&rect, 1) = vec_add(MAKE_VEC2(X(diff), 0), a);  
  VERTEX(&rect, 2) = b;
  VERTEX(&rect, 3) = vec_add(MAKE_VEC2(0, Y(diff)), a);

  rect.num_vertices = 4;
  return rect;
}

Polygon isoscelesTriangleCentered(float base, float height) {
  Polygon res = isoscelesTriangle(base, height, make_vec(0,0,0));
  vector center = vec_mul(polygonCenter(&res), -1);
  polygonMove(&res, center);

  return res;
}

Polygon almostRegularPolygon(int num_vertices, float size, float diviation) {
  Polygon res = regularPolygon(num_vertices, size);
  for (int i=0; i< num_vertices; i++) {
    vector *v = &VERTEX(&res, i);
    *v = vec_add(*v, MAKE_VEC2(rand() % 35, rand() %35));
  }

  return res;
}

Polygon regularPolygon(int num_vertices, float size) {
  Polygon res;

  res.num_vertices = num_vertices;

  int j;
  for (int i = 0; i < num_vertices; i++) {
    if (i % 2 == 0)
      j = num_vertices - i/2;
    else
      j = (i + 1) / 2;
    float rot = ((float)j * 360) / num_vertices;
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

Polygon transform(Polygon *poly, matrix trans) { 
  Polygon trans_poly;

  trans_poly.num_vertices = poly->num_vertices;
  for (int i=0; i < poly->num_vertices; i++) {
    VERTEX(&trans_poly, i) = vec_transform(trans, VERTEX(poly, i));
  }

  return trans_poly;
}

bool checkCollision(Polygon *poly1, vector pos1, Polygon *poly2, vector pos2, Polygon *collision) {
  Polygon rect1 = boundingBox(poly1);
  Polygon rect2 = boundingBox(poly2);

  float a1, a2, max_a;
  float b1, b2, min_b;

  vector max_a_vec;
  vector min_b_vec;

  for (int i = 0; i < 2; i++) {
    a1 = VGET(VERTEX(&rect1, 0), i) + VGET(pos1, i);
    b1 = VGET(VERTEX(&rect1, 2), i) + VGET(pos1, i);
    
    a2 = VGET(VERTEX(&rect2, 0), i) + VGET(pos2, i);
    b2 = VGET(VERTEX(&rect2, 2), i) + VGET(pos2, i);

    max_a = a1;
    if (a2 > a1)
      max_a = a2;

    min_b = b1;
    if (b2 < b1)
      min_b = b2;
    
    if (max_a > min_b)
      return false;

    VGET(max_a_vec, i) = max_a;
    VGET(min_b_vec, i) = min_b;
  }

  if (collision) {
    *collision = rectangle(max_a_vec, min_b_vec);
  }

  return true;
}

void centralizePolygon(Polygon *poly) {
  vector center = polygonCenter(poly);

  for (int i=0; i<poly->num_vertices;i++) {
    vector *p = &VERTEX(poly, i);
    *p = vec_sub(*p, center);
  }
}
