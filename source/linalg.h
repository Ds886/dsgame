struct vec2 {
    float x;
    float y;
};

struct vec3 {
    float x;
    float y;
    float z;
};

struct vec2 vec2_mul(struct vec2 v1, int scalar);

struct vec2 vec2_add(struct vec2 v1, struct vec2 v2);

struct vec2 vec2_sub(struct vec2 v1, struct vec2 v2);

struct vec2 vec2_div(struct vec2 v1, float scalar);

struct vec3 vec3_mul(struct vec3 v1, float scalar);

struct vec3 vec3_add(struct vec3 v1, struct vec3 v2);

struct vec3 vec3_sub(struct vec3 v1, struct vec3 v2);

struct vec3 vec3_div(struct vec3 v1, float scalar);
