#include "game.h"

#include <nds.h>

void crossScreen(struct vec2 *pos) {
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
GameObj newTriangle(Triangle tri, struct vec2 pos, float accel, float rotation_speed, Color color) {
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

  
  struct vec2 vecPosition = {GAME_SCREEN_WIDTH / 2 - PLAYER_HALF_WIDTH, GAME_SCREEN_HEIGHT / 2 - PLAYER_HALF_HEIGHT};


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
  translate_matrix_2d(m, diff_x, diff_y);
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

#define PRINT_MAT(m) printf("[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n", \
    (m)[0][0], (m)[0][1], (m)[0][2], \
    (m)[1][0], (m)[1][1], (m)[1][2], \
    (m)[2][0], (m)[2][1], (m)[2][2]);
  
Game *gameRender(Game *game) {
  matrix m, n, t;
  printf("rotation: %f\n", game->ship->rotation);
  rotation_matrix_2d(m, game->ship->rotation);
  translate_matrix_2d(n, game->ship->position.x, game->ship->position.y);
  mat_mul(t, m, n);
  
  PRINT_MAT(t);

  renderPolygon(&game->ship->triangle, t, &game->ship->color);
  
  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

