#include "game.h"

#if USE_MOCK != 0

#include <nds.h>

#define MOCK_PARAMS \
  NULL, NULL, \
  0, 0, 0, 0, 0, 0, 0, \
  make_vec(0, 0, 0)

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
    Color player_color) {
  
  printf("MOCK STARTED!");
  
  return game;
}

Game *gameLogic(Game *game, uint16_t keys) {
  return game;
}

Game *gameRender(Game *game) {
  printf("MOCKING!");
  
  return game;
}

Game *gameEnd(Game *game) {
  return game;
}

#endif
