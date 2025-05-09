#include "game.h"

#include <nds.h>


void crossScreen(vec2 *pos) {
    coord *cpos = COORD(pos);
    if (cpos->x < 0) {
        cpos->x += GAME_SCREEN_WIDTH;
    }
    if (cpos->x >= GAME_SCREEN_WIDTH) {
        cpos->x -= GAME_SCREEN_WIDTH;
    }

    if (cpos->y < 0) {
        cpos->y += GAME_SCREEN_HEIGHT;
    }
    if (cpos->y >= GAME_SCREEN_HEIGHT) {
        cpos->y -= GAME_SCREEN_HEIGHT;
    }
}
GameObj newTriangle(Triangle tri, vec2 pos, float accel, float rotation_speed, Color color) {
    GameObj ret;

    ret.triangle = tri;
    ret.position = pos;
    ret.velocity = 0;
    ret.acceleration = accel;
    ret.rotation = 0;
    ret.rotation_speed = rotation_speed;
    ret.color = color;

    return ret;
}


Game *gameStart(
    Game *game,
    GameObj *ship,
    float friction,
    float player_accel,
    float player_rotation_speed,
    Color player_color) 
{
  game->frame = 0;
  game->friction = friction;
  game->ship = ship;

  
  vec2 vecPosition = make_vec(
      GAME_SCREEN_WIDTH / 2 - PLAYER_HALF_WIDTH,
      GAME_SCREEN_HEIGHT / 2 - PLAYER_HALF_HEIGHT,
      0
  );


  *ship = newTriangle (
        isoscelesTriangle(PLAYER_WIDTH, PLAYER_HEIGHT),
        vecPosition,
        player_accel,
        player_rotation_speed,
        player_color
    );

  
  return game;
}


void set_in_position(GameObj *poly) {
  matrix m;
  float diff_x = X(poly->position) - X(poly->triangle.a);
  float diff_y = Y(poly->position) - Y(poly->triangle.a);
  m = translate_matrix_2d(diff_x, diff_y);
  transform(&poly->triangle, m);
}

Game *gameLogic(Game *game, uint16_t keys) {
  GameObj *ship = game->ship;
  s16 bin_rotation = degreesToAngle(-ship->rotation);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);

  if (ship->velocity > 0)
    ship->velocity -= 0.01;

  if (ship->velocity < 0)
    ship->velocity += 0.01;

  if (keys & KEY_LEFT) {
    ship->rotation += 3;
  }

  if (keys & KEY_RIGHT) {
    ship->rotation -= 3;
  }

  if (keys & KEY_UP) {
    ship->velocity += ship->acceleration;
  }

  if (keys & KEY_DOWN) {
    ship->velocity -= ship->acceleration;
  }
  
  X(ship->position) -= ship->velocity * sin;
  Y(ship->position) -= ship->velocity * cos;
   
  game->frame++;

  printf("ship velo: %f\n ship accel: %f\n", ship->velocity, ship->acceleration);
  return game;
}

Game *gameRender(Game *game) {
  vector pos = game->ship->position;
  matrix rotate = rotation_matrix_2d(game->ship->rotation);
 
  renderPolygon(&game->ship->triangle, game->ship->position, rotate, &game->ship->color);

  PRINT_VEC(pos);
  PRINT_MAT(rotate);

  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

