#include "linalg.h"

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

