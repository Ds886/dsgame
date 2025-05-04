#ifndef GAME_H
#define GAME_H

#include "polygon.h"

typedef struct game_obj_t {
    Triangle triangle;
    struct vec2 position;
    struct vec2 velocity;
    float acceleration;
    float rotation;
    float rotation_speed;
} GameObj;


typedef struct game_t {
  int frame;
} Game;

Game *gameStart(Game *game);
Game *gameLogic(Game *game);
Game *gameRender(Game *game);
Game *gameEnd(Game *game);

#endif
