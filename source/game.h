#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "nds/input.h"

#include "polygon.h"
#include "linalg.h"
#include "renderer.h"


#define GAME_SCREEN_WIDTH 256
#define GAME_SCREEN_HEIGHT 192

#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 16
#define PLAYER_HALF_WIDTH (PLAYER_WIDTH / 2)
#define PLAYER_HALF_HEIGHT (PLAYER_HEIGHT / 2)

#define ASTROID_COLOR make_vec(0.5, 0.5, 0.15)
#define ASTRO_NUM_VERTICES 7
#define ASTROID_ANIMATION_SPEED 2
#define SHOOT_SIZE 10
#define ASTROID_SPLIT_SCALE 0.5
#define ASTROID_SPLIT_NUM_PARTITIONS 2


#define CHANGED_KEYS(g, k)  (((g)->keys) ^ (k))
#define PRESSED_KEYS(g, k)  ((~(g)->keys) & (k))
#define RELEASED_KEYS(g, k) (((g)->keys) & (~k))

#define OUT_OF_BOUNDS(p, a) ((X(p) < 0) || (Y(p) < 0) || (X(p) >= GAME_SCREEN_WIDTH+(a)) ||( Y(p) >= GAME_SCREEN_HEIGHT+(a)))

//TODO: this will fail after 2^32 frames!
// must take care of overflow!
#define ELAPSED_BETWEEN(a, b) ((b) - (a))

#define SHIP_ANIMATION_TIME 10

struct ship_t;

enum obj_state {
    OBJ_STATE_NONE,
    OBJ_STATE_NORMAL,
    OBJ_STATE_DYING,
    OBJ_STATE_DEAD,
    OBJ_STATE_BORN,
    OBJ_STATE_READY_REBORN
};

typedef struct game_obj_t {
    bool alive;
    Polygon polygon;
    vec2 position;
    float velocity;
    float rotation;
    bool collidable;
    Color color;
    int state_time;
    enum obj_state state;
} GameObj;

typedef struct shoot_t {
    GameObj obj;
    struct ship_t *shooter;
} Shoot;


typedef struct ship_t {
    GameObj obj;
    float acceleration;
    float rotation_speed;
    float max_velocity;
    Shoot *shoots;
    float shoot_freq;
    int max_num_shoots;
    int lives;
} Ship;

typedef struct astroid_t {
    GameObj obj;
    int stage;
} Astroid;

typedef struct game_t {
  int frame;
  uint16_t keys;
  float friction;
  Ship *ship;
  Astroid *astroids;
  int max_num_astroids;
  float astroid_size;
  float astroid_velocity;
  int astroid_num_stages;
} Game;

Game *gameStart(
    Game *game,
    Ship *ship,
    Astroid *astroids,
    Shoot *shoots,
    int max_num_shoots,
    float initial_shoot_freq,
    int max_num_astroids,
    int astroid_num_stages,
    float astroid_initial_size,
    float astroid_velocity,
    float friction,
    float player_accel,
    float player_rotation_speed,
    float player_max_velocity,
    int player_lives,
    Color player_color);
Game *gameLogic(Game *game, uint16_t keys);
Game *gameRender(Game *game);
Game *gameEnd(Game *game);

#endif
