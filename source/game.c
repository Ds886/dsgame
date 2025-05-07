#include "game.h"

#include <nds.h>

void crossScreen(vec2 *pos) {
    if (pos->x < 0) {
        pos->x += GAME_SCREEN_WIDTH;
    }
    if (pos->x >= GAME_SCREEN_WIDTH) {
        pos->x -= GAME_SCREEN_WIDTH;
    }

    if (pos->y < 0) {
        pos->y += GAME_SCREEN_HEIGHT;
    }
    if (pos->y >= GAME_SCREEN_HEIGHT) {
        pos->y -= GAME_SCREEN_HEIGHT;
    }
}
GameObj newTriangle(Triangle tri, vec2 pos, float accel, float rotation_speed, Color color) {
    GameObj ret;

    ret.triangle = tri;
    ret.position = pos;
    ret.velocity.x = 0;
    ret.velocity.y = 0;
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

  
  vec2 vecPosition = {GAME_SCREEN_WIDTH / 2 - PLAYER_HALF_WIDTH, GAME_SCREEN_HEIGHT / 2 - PLAYER_HALF_HEIGHT, 0};


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
  float diff_x = poly->position.x - poly->triangle.a.x;
  float diff_y = poly->position.y - poly->triangle.a.y;
  m = translate_matrix_2d(diff_x, diff_y);
  transform(&poly->triangle, m);
}

Game *gameLogic(Game *game, uint16_t keys) {
  if (keys & KEY_LEFT) {
    game->ship->position.x+=1.4;
  }

  if (keys & KEY_RIGHT) {
    game->ship->position.x-=1.4;
  }

  if (keys & KEY_UP) {
    game->ship->position.y-=1.4;
  }

  if (keys & KEY_DOWN) {
    game->ship->position.y+=1.4;
  }

  if (keys & KEY_X) {
    game->ship->rotation += 1;
  }

  if (keys & KEY_Y) {
    game->ship->rotation -= 1;
  }

  game->frame++;
  return game;
}

#define PRINT_MAT(m) printf("[%f\t%f\t%f]\n[%f\t%f\t%f]\n[%f\t%f\t%f];\n\n", \
     MGET(m, 0, 0), MGET(m, 0, 1), MGET(m, 0, 2), \
     MGET(m, 1, 0), MGET(m, 1, 1), MGET(m, 1, 2), \
     MGET(m, 2, 0), MGET(m, 2, 1), MGET(m, 2, 2));

#define PRINT_VEC(v) printf("[%f\t%f\t%f];\n\n", (v).x, (v).y, (v).z)

Game *gameRender(Game *game) {
  vector pos = game->ship->position;
  vector t = {50, 50, 1};
  matrix rotate = rotation_axis_matrix_2d(game->ship->rotation, t);
  matrix move = translate_matrix_2d(pos.x, pos.y);

  matrix m = mat_mul(move, rotate);
 
  renderPolygon(&game->ship->triangle, m, &game->ship->color);

  PRINT_VEC(pos);
  PRINT_MAT(m);

  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

