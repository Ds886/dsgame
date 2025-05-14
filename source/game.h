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

typedef struct game_obj_t {
    bool alive;
    Polygon polygon;
    vec2 position;
    float velocity;
    float acceleration;
    float rotation;
    float rotation_speed;
    float max_velocity;
    Color color;
    struct game_obj_t *next;
} GameObj;


typedef struct game_t {
  int frame;
  float friction;
  GameObj *ship;
  GameObj *astroids;
  int num_astroids;
  int max_num_astroids;
  float astroid_size;
  float astroid_velocity;
} Game;

Game *gameStart(
    Game *game,
    GameObj *ship,
    GameObj *astroids,
    int max_num_astroids,
    float astroid_initial_size,
    float astroid_velocity,
    float friction,
    float player_accel,
    float player_rotation_speed,
    float player_max_velocity,
    Color player_color);
Game *gameLogic(Game *game, uint16_t keys);
Game *gameRender(Game *game);
Game *gameEnd(Game *game);

#endif
