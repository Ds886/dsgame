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

matrix makeMatrix(float a00, float a01, float a02, 
                  float a10, float a11, float a12,
                  float a20, float a21, float a22) {
    matrix m;
    
    MGET(m, 0, 0) = a00;
    MGET(m, 0, 1) = a01;
    MGET(m, 0, 2) = a02;
    MGET(m, 1, 0) = a10;
    MGET(m, 1, 1) = a11;
    MGET(m, 1, 2) = a12;
    MGET(m, 2, 0) = a20;
    MGET(m, 2, 1) = a21;
    MGET(m, 2, 2) = a22;

    return m;
}

matrix mat_add(matrix m1, matrix m2) {
    matrix res;
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            MGET(res,i,j) = MGET(m1,i,j) + MGET(m2,i,j);
    return res;
}

matrix mat_sub(matrix m1, matrix m2) {
    matrix res;
    res = mat_scale(m2, -1);
    res = mat_add(m1, res);
    return res;
}

matrix mat_mul(matrix m1, matrix m2) {
    matrix res;
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++) {
            MGET(res,i,j) = 0;
            for (int k = 0; k < MAT_SIZE; k++)
                MGET(res, i, j) += MGET(m1, i, k) * MGET(m2, k, j);
        }
    return res;
}

matrix mat_scale(matrix m1, float scalar) {
    matrix res;
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            MGET(res, i, j) = MGET(m1, i, j) * scalar;
    return res;
}

matrix mat_fill(float val) {
    matrix res;
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            MGET(res, i, j) = val;
    return res;
}

matrix mat_zeros() {
    return mat_fill(0);
}

matrix mat_identity() {
    matrix res;
    for (int i = 0; i < MAT_SIZE; i++)
        for (int j = 0; j < MAT_SIZE; j++)
            MGET(res, i, j) = (i == j) ? 1 : 0;
    return res;
}

matrix rotation_axis_matrix_2d(float degrees, struct vec3 axis) {
    matrix res;
    matrix m, n, t, r;
    m = translate_matrix_2d(-axis.x, -axis.y);
    n = translate_matrix_2d(+axis.x, +axis.y);
    t = rotation_matrix_2d(degrees);

    r = mat_mul(m, t);
    res = mat_mul(r, n);

    return res;
}

matrix rotation_matrix_2d(float degrees) {
    s16 bin_rotation = degreesToAngle(degrees);
    float cos = fixedToFloat(cosLerp(bin_rotation), 12);
    float sin = fixedToFloat(sinLerp(bin_rotation), 12);

    return makeMatrix(cos, -sin, 0,
                      sin,  cos, 0,
                      0  ,  0  , 1);
}

struct vec3 vec3_transform(matrix m, struct vec3 v) {
    struct vec3 res;

    res.x = MGET(m,0,0) * v.x + MGET(m,0,1) * v.y + MGET(m,0,2) * v.z;
    res.y = MGET(m,1,0) * v.x + MGET(m,1,1) * v.y + MGET(m,1,2) * v.z;
    res.z = MGET(m,2,0) * v.x + MGET(m,2,1) * v.y + MGET(m,2,2) * v.z;

    return res;
}

matrix translate_matrix_2d(float x, float y) {
    return makeMatrix(1, 0, x,
                      0, 1, y,
                      0, 0, 1);
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
    
    m = rotation_matrix_2d(degrees);
    v3 = vec3_transform(m, v3);

    return vec_reduce_dim(v3);
}
