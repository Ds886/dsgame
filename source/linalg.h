#ifndef LINLANG_H
#define LINLANG_H 1

struct vec2 {
    float x;
    float y;
};

struct vec3 {
    float x;
    float y;
    float z;
};

#define MAT_SIZE 3

typedef float matrix[MAT_SIZE][MAT_SIZE];

struct vec2 vec2_mul(struct vec2 v1, int scalar);

struct vec2 vec2_add(struct vec2 v1, struct vec2 v2);

struct vec2 vec2_sub(struct vec2 v1, struct vec2 v2);

struct vec2 vec2_div(struct vec2 v1, float scalar);

struct vec3 vec3_mul(struct vec3 v1, float scalar);

struct vec3 vec3_add(struct vec3 v1, struct vec3 v2);

struct vec3 vec3_sub(struct vec3 v1, struct vec3 v2);

struct vec3 vec3_div(struct vec3 v1, float scalar);

struct vec3 vec3_transform(matrix m, struct vec3 v);

struct vec2 vec2_rotate(struct vec2 v, float degrees);

void mat_add(matrix res, matrix m1, matrix m2);

void mat_sub(matrix res, matrix m1, matrix m2);

void mat_mul(matrix res, matrix m1, matrix m2);

void mat_scale(matrix res, matrix m1, float scalar);

void mat_zeros(matrix res);

void rotation_matrix_2d(matrix res, float degrees);

void rotation_axis_matrix_2d(matrix res, float degrees, struct vec3 axis);

void translate_matrix_2d(matrix res, float x, float y);

#endif
