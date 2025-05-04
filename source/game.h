#ifndef GAME_H
#define GAME_H

#include "polygon.h"
#include "linalg.h"


#define GAME_SCREEN_WIDTH 256
#define GAME_SCREEN_HEIGHT 192

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_HALF_WIDTH (PLAYER_WIDTH / 2)
#define PLAYER_HALF_HEIGHT (PLAYER_HEIGHT / 2)

typedef struct vec3 Color;

typedef struct game_obj_t {
    Triangle triangle;
    struct vec2 position;
    struct vec2 velocity;
    float acceleration;
    float rotation;
    float rotation_speed;
    Color col;
} GameObj;


typedef struct game_t {
  int frame;
  GameObj *ship;
} Game;

Game *gameStart(
    Game *game,
    GameObj *ship,
    int player_accel,
    int player_rotation_speed,
    Color player_color);
Game *gameLogic(Game *game);
Game *gameRender(Game *game);
Game *gameEnd(Game *game);

#endif
