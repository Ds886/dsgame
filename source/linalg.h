#ifndef LINLANG_H
#define LINLANG_H 1

#define VEC_SIZE 3

#define VGET(a, i) ((a).v[i])
#define X(a) VGET(a, 0)
#define Y(a) VGET(a, 1)
#define Z(a) VGET(a, 2)

#define ABS(x) (((x) <= 0) ? (-(x)) : (x))

#define ZERO_VEC (make_vec(0, 0, 0))
#define MAKE_VEC2(x, y) (make_vec(x, y, 0))
#define PRINT_MAT(m) printf("[%f\t%f\t%f]\n[%f\t%f\t%f]\n[%f\t%f\t%f];\n\n", \
     MGET(m, 0, 0), MGET(m, 0, 1), MGET(m, 0, 2), \
     MGET(m, 1, 0), MGET(m, 1, 1), MGET(m, 1, 2), \
     MGET(m, 2, 0), MGET(m, 2, 1), MGET(m, 2, 2));

#define PRINT_VEC(v) printf("[%f\t%f\t%f];\n\n", X(v), Y(v), Z(v))

typedef struct vector_t {
    float v[VEC_SIZE];
} vector;

#define MAT_SIZE VEC_SIZE

#define MGET(m, x, y) ((m).a[x][y])

typedef struct matrix_t {
    float a[MAT_SIZE][MAT_SIZE];
} matrix;

typedef vector vec2;


typedef struct coord3_t {
    float x;
    float y;
    float z;
} coord;

#define COORD(p) ((coord *)(p))

vector make_vec(float x, float y, float z);

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

matrix mat_identity();

matrix mat_scaling(float strech);

matrix rotation_matrix_2d(float degrees);

matrix rotation_axis_matrix_2d(float degrees, vector axis);

matrix translate_matrix_2d(float x, float y);

#endif
