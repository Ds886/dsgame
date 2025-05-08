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
    int player_accel,
    int player_rotation_speed,
    Color player_color) 
{
  game->frame = 0;
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

  s16 bin_rotation = degreesToAngle(-game->ship->rotation);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);

//  if (game->ship->velocity > 0)
//    game->ship->velocity -= 0.1;


  if (keys & KEY_LEFT) {
    game->ship->rotation += 3;
  }

  if (keys & KEY_RIGHT) {
    game->ship->rotation -= 3;
  }

  if (keys & KEY_UP) {
    game->ship->velocity += game->ship->acceleration;
    X(game->ship->position) -= game->ship->velocity * sin;
    Y(game->ship->position) -= game->ship->velocity * cos;
  }

  if (keys & KEY_DOWN) {
    game->ship->velocity -= game->ship->acceleration;
    X(game->ship->position) += game->ship->velocity * sin;
    Y(game->ship->position) += game->ship->velocity * cos;
  }
  
  game->frame++;
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

