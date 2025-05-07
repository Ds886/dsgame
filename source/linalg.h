#ifndef LINLANG_H
#define LINLANG_H 1

struct vec3 {
    float x;
    float y;
    float z;
};

#define MAT_SIZE 3

#define MGET(m, x, y) ((m).a[x][y])

typedef struct matrix_t {
    float a[MAT_SIZE][MAT_SIZE];
} matrix;

typedef struct vec3 vec2;

matrix makeMatrix(float, float, float, 
                  float, float, float,
                  float, float, float);

struct vec3 vec3_mul(struct vec3 v1, float scalar);

struct vec3 vec3_add(struct vec3 v1, struct vec3 v2);

struct vec3 vec3_sub(struct vec3 v1, struct vec3 v2);

struct vec3 vec3_div(struct vec3 v1, float scalar);

struct vec3 vec3_transform(matrix m, struct vec3 v);

matrix mat_add(matrix m1, matrix m2);

matrix mat_sub(matrix m1, matrix m2);

matrix mat_mul(matrix m1, matrix m2);

matrix mat_scale(matrix m1, float scalar);

matrix mat_zeros();

matrix rotation_matrix_2d(float degrees);

matrix rotation_axis_matrix_2d(float degrees, struct vec3 axis);

matrix translate_matrix_2d(float x, float y);

#endif
