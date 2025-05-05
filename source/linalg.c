#include "linalg.h"
#include "nds/arm9/trig_lut.h"

struct vec2 vec2_mul(struct vec2 v1, int scalar){
    struct vec2 temp = {v1.x * scalar,
                        v1.y * scalar};
    return temp;
}

struct vec2 vec2_add(struct vec2 v1, struct vec2 v2){
    struct vec2 temp = {v1.x + v2.x, v1.y + v2.y};
    return temp;
}

struct vec2 vec2_sub(struct vec2 v1, struct vec2 v2){
    struct vec2 temp = {v1.x - v2.x, v1.y - v2.y};
    return temp;
}

struct vec2 vec2_div(struct vec2 v1, float scalar){
    struct vec2 temp = {v1.x / scalar,
                        v1.y / scalar};
    return temp;
}

struct vec3 vec3_mul(struct vec3 v1, float scalar){
    struct vec3 temp = {v1.x * scalar,
                        v1.y * scalar,
                        v1.z * scalar};
    return temp;
}

struct vec3 vec3_add(struct vec3 v1, struct vec3 v2){
    struct vec3 temp = {v1.x + v2.x,
                        v1.y + v2.y,
                        v1.z + v2.z};
    return temp;
}
struct vec3 vec3_sub(struct vec3 v1, struct vec3 v2){
    struct vec3 temp = {v1.x - v2.x,
                        v1.y - v2.y,
                        v1.z - v2.z};
    return temp;
}
struct vec3 vec3_div(struct vec3 v1, float scalar){
    struct vec3 temp = {v1.x / scalar,
                        v1.y / scalar,
                        v1.z / scalar};
    return temp;
}

void mat_add(matrix res, matrix m1, matrix m2) {
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            res[i][j] = m1[i][j] + m2[i][j];
}

void mat_sub(matrix res, matrix m1, matrix m2) {
    mat_scale(res, m2, -1);
    mat_add(res, m1, res);
}

void mat_mul(matrix res, matrix m1, matrix m2) {
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++) {
            res[i][j] = 0;
            for (int k = 0; k < MAT_SIZE; k++)
                res[i][j] += m1[i][k] * m2[k][j];
        }
}

void mat_scale(matrix res, matrix m1, float scalar) {
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            res[i][j] = m1[i][j] * scalar;
}

void mat_fill(matrix res, float val) {
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            res[i][j] = val;
}

void mat_zeros(matrix res) {
    mat_fill(res, 0);
}

void mat_identity(matrix res) {
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            res[i][j] = (i == j) ? 1 : 0;
}

void rotation_axis_matrix_2d(matrix res, float degrees, struct vec3 axis) {
    matrix m, n, t, r;
    translate_matrix_2d(m, -axis.x, -axis.y);
    translate_matrix_2d(n, +axis.x, +axis.y);
    rotation_matrix_2d(t, degrees);

    mat_mul(r, m, t);
    mat_mul(res, r, n);
}

void rotation_matrix_2d(matrix res, float degrees) {
    s16 bin_rotation = degreesToAngle(degrees);
    float cos = fixedToFloat(cosLerp(bin_rotation), 12);
    float sin = fixedToFloat(sinLerp(bin_rotation), 12);

    mat_identity(res);
    res[0][0] = cos;
    res[0][1] = -sin;
    res[1][0] = sin;
    res[1][1] = cos;
}

struct vec3 vec3_transform(matrix m, struct vec3 v) {
    struct vec3 res;

    res.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    res.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    res.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;

    return res;
}

void translate_matrix_2d(matrix res, float x, float y) {
    res[0][0] = 1;
    res[0][1] = 0;
    res[0][2] = x;

    res[1][0] = 0;
    res[1][1] = 1;
    res[1][2] = y;

    res[2][0] = 0;
    res[2][1] = 0;
    res[2][2] = 1;    
}

struct vec3 vec_inc_dim(struct vec2 v) {
    struct vec3 v3;
    v3.x = v.x;
    v3.y = v.y;
    v3.z = 0;
    return v3;
}

struct vec2 vec_reduce_dim(struct vec3 v) {
    struct vec2 v2;
    v2.x = v.x;
    v2.y = v.y;
    return v2;
}

struct vec2 vec2_rotate(struct vec2 v, float degrees) {
    struct vec3 v3 = vec_inc_dim(v);
    matrix m;
    
    rotation_matrix_2d(m, degrees);
    vec3_transform(m, v3);

    return vec_reduce_dim(v3);
}
