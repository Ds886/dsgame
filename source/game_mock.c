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
  Polygon rect1 = rectangle(MAKE_VEC2(50, 35), MAKE_VEC2(200,90));
  Polygon rect2 = rectangle(MAKE_VEC2(150, 75), MAKE_VEC2(250,140));

  Polygon collision;

  renderPolygon(&rect1, ZERO_VEC, BLUE);
  renderPolygon(&rect2, ZERO_VEC, RED);

  if (checkCollision(&rect1, ZERO_VEC, &rect2, ZERO_VEC, &collision)) {
      printf("Collide!\n");
      renderPolygon(&collision, ZERO_VEC, YELLOW);
  } else {
    printf("no collision :(\n");
  }

  
  return game;
}

Game *gameEnd(Game *game) {
  return game;
}

#endif
