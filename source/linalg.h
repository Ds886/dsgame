#ifndef LINLANG_H
#define LINLANG_H 1

typedef struct vector_t {
    float x;
    float y;
    float z;
} vector;

#define MAT_SIZE 3

#define MGET(m, x, y) ((m).a[x][y])

typedef struct matrix_t {
    float a[MAT_SIZE][MAT_SIZE];
} matrix;

typedef vector vec2;

matrix makeMatrix(float, float, float, 
                  float, float, float,
                  float, float, float);

vector vec_mul(vector v1, float scalar);

vector vec_add(vector v1, vector v2);

vector vec_sub(vector v1, vector v2);

vector vec_div(vector v1, float scalar);

vector vec_transform(matrix m, vector v);

matrix mat_add(matrix m1, matrix m2);

matrix mat_sub(matrix m1, matrix m2);

matrix mat_mul(matrix m1, matrix m2);

matrix mat_scale(matrix m1, float scalar);

matrix mat_zeros();

matrix rotation_matrix_2d(float degrees);

matrix rotation_axis_matrix_2d(float degrees, vector axis);

matrix translate_matrix_2d(float x, float y);

#endif
