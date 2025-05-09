#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "nds/input.h"

#include "polygon.h"
#include "linalg.h"
#include "renderer.h"


#define GAME_SCREEN_WIDTH 256
#define GAME_SCREEN_HEIGHT 192

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_HALF_WIDTH (PLAYER_WIDTH / 2)
#define PLAYER_HALF_HEIGHT (PLAYER_HEIGHT / 2)

typedef struct game_obj_t {
    Triangle triangle;
    vec2 position;
    float velocity;
    float acceleration;
    float rotation;
    float rotation_speed;
    Color color;
} GameObj;


typedef struct game_t {
  int frame;
  float friction;
  GameObj *ship;
} Game;

Game *gameStart(
    Game *game,
    GameObj *ship,
    float friction,
    float player_accel,
    float player_rotation_speed,
    Color player_color);
Game *gameLogic(Game *game, uint16_t keys);
Game *gameRender(Game *game);
Game *gameEnd(Game *game);

#endif
