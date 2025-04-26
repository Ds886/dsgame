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
