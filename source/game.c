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
GameObj newTriangle(
    Triangle tri, vec2 pos,
    float accel, float rotation_speed,
    float max_velocity, Color color
) {
    GameObj ret;

    ret.triangle = tri;
    ret.position = pos;
    ret.velocity = 0;
    ret.acceleration = accel;
    ret.rotation = 0;
    ret.rotation_speed = rotation_speed;
    ret.max_velocity = max_velocity;
    ret.color = color;

    return ret;
}


Game *gameStart(
    Game *game,
    GameObj *ship,
    GameObj *astroids,
    int max_num_astroids,
    float friction,
    float player_accel,
    float player_rotation_speed,
    float player_max_velocity,
    Color player_color) 
{
  game->frame = 0;
  game->friction = friction;
  game->ship = ship;
  game->astroids = astroids;
  game->max_num_astroids = max_num_astroids;
  game->num_astroids = 0;

  
  vec2 vecPosition = make_vec(
      GAME_SCREEN_WIDTH / 2 - PLAYER_HALF_WIDTH,
      GAME_SCREEN_HEIGHT / 2 - PLAYER_HALF_HEIGHT,
      0
  );


  *ship = newTriangle (
        isoscelesTriangleCentered(PLAYER_WIDTH, PLAYER_HEIGHT),
        vecPosition,
        player_accel,
        player_rotation_speed,
        player_max_velocity,
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

void shipGameLogic(GameObj *ship, float gameFriction, uint16_t keys) {
  s16 bin_rotation = degreesToAngle(-ship->rotation);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);

  if (ship->velocity > 0)
    ship->velocity -= gameFriction;

  if (ship->velocity < 0)
    ship->velocity += gameFriction;

  if (keys & KEY_LEFT) {
    ship->rotation -= ship->rotation_speed;
  }

  if (keys & KEY_RIGHT) {
    ship->rotation += ship->rotation_speed;;
  }

  if (keys & KEY_UP) {
    if (ship->velocity < ship->max_velocity)
      ship->velocity += ship->acceleration;
  }

  if (keys & KEY_DOWN) {
    if (ship->velocity > -ship->max_velocity)
      ship->velocity -= ship->acceleration;
  }

  if (ABS(ship->velocity) > 0.1) {
    X(ship->position) -= ship->velocity * sin;
    Y(ship->position) -= ship->velocity * cos;
  }

  crossScreen(&ship->position);

  printf("ship velo: %f\n ship accel: %f\n", ship->velocity, ship->acceleration);
}

void astroidGameLogic(GameObj *astro) {
  s16 bin_rotation = degreesToAngle(-astro->rotation);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);

  if (ABS(astro->velocity) > 0.1) {
    X(astro->position) += astro->velocity * sin;
    Y(astro->position) += astro->velocity * cos;
  }
}

void spawnAstroid(Game *game) {
  GameObj *astro = &game->astroids[game->num_astroids++];

  astro->triangle = isoscelesTriangleCentered(40, 40);
  astro->position = MAKE_VEC2(10, 10);
  astro->velocity = 7;
  astro->acceleration = 0;
  astro->rotation = -30;
  astro->rotation_speed = 0;
  astro->max_velocity = 30;
  astro->color = make_vec(0, 1, 1);
}

Game *gameLogic(Game *game, uint16_t keys) {
  shipGameLogic(game->ship, game->friction, keys);
  if (game->frame % 80 == 79 && game->num_astroids < game->max_num_astroids) {
    spawnAstroid(game);
  }

  for (int i = 0; i < game->num_astroids; i++) {
    astroidGameLogic(&game->astroids[i]);
  }
  
  printf("num astroids: %d/%d\n", game->num_astroids, game->max_num_astroids);
  game->frame++;
  return game;
}

Game *gameRender(Game *game) {
  vector pos = game->ship->position;
  matrix rotate = rotation_matrix_2d(game->ship->rotation);
 
  renderPolygonTransformed(&game->ship->triangle, game->ship->position, rotate, &game->ship->color);
  for (int i = 0; i < game->num_astroids; i++) {
    GameObj *astro = &game->astroids[i];
    renderPolygon(&astro->triangle, astro->position, &astro->color);
  }

  PRINT_VEC(pos);
  PRINT_MAT(rotate);

  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

