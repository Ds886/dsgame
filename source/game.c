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

GameObj newGameObj(
    Polygon poly, vec2 pos, float velocity,
    float accel, float rotation, float rotation_speed,
    float max_velocity, Color color
) {
    GameObj ret;

    ret.polygon = poly;
    ret.position = pos;
    ret.velocity = velocity;
    ret.acceleration = accel;
    ret.rotation = rotation;
    ret.rotation_speed = rotation_speed;
    ret.max_velocity = max_velocity;
    ret.color = color;
    ret.alive = true;
    ret.next = NULL;

    return ret;
}

GameObj newIdleGameObj(
    Polygon poly, vec2 pos,
    float accel, float rotation_speed,
    float max_velocity, Color color
) {
  return newGameObj(poly, pos, 0, accel, 0, rotation_speed, max_velocity, color);
}


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
    Color player_color) 
{
  game->frame = 0;
  game->friction = friction;
  game->ship = ship;
  game->astroids = astroids;
  game->max_num_astroids = max_num_astroids;
  game->num_astroids = 0;
  game->astroid_size = astroid_initial_size;
  game->astroid_velocity = astroid_velocity;
  
  vec2 vecPosition = make_vec(
      GAME_SCREEN_WIDTH / 2 - PLAYER_HALF_WIDTH,
      GAME_SCREEN_HEIGHT / 2 - PLAYER_HALF_HEIGHT,
      0
  );


  *ship = newIdleGameObj (
        isoscelesTriangleCentered(PLAYER_WIDTH, PLAYER_HEIGHT),
        vecPosition,
        player_accel,
        player_rotation_speed,
        player_max_velocity,
        player_color
    );

  
  return game;
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
}

void astroidGameLogic(GameObj *astro) {
  if (!astro->alive)
    return;

  s16 bin_rotation = degreesToAngle(-astro->rotation);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);

  if (ABS(astro->velocity) > 0.1) {
    X(astro->position) += astro->velocity * sin;
    Y(astro->position) += astro->velocity * cos;
  }
  
  if (X(astro->position) > GAME_SCREEN_WIDTH+40 || Y(astro->position) > GAME_SCREEN_HEIGHT+40) {
    astro->alive = false;
  }
}

void spawnAstroid(Game *game) {
  GameObj *astro = &game->astroids[game->num_astroids++];
  vector pos;

  float rot = (float)(random() % 360);
  
  s16 bin_rotation = degreesToAngle(rot);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);
  pos = MAKE_VEC2(GAME_SCREEN_WIDTH/2, GAME_SCREEN_HEIGHT/2);
  float fact = game->astroid_size + GAME_SCREEN_WIDTH / 2;
  pos = vec_add(pos, MAKE_VEC2(cos * fact, sin * fact));

  *astro = newGameObj(
    almostRegularPolygon(ASTRO_NUM_VERTICES, game->astroid_size, 0),
    pos, game->astroid_velocity, 0, rot + 90, 0, 0, ASTROID_COLOR);
}

void cleanDeadObjs(GameObj *objs, int *num_objs) {
  int num_dead_objs = 0;
  for (int i = *num_objs-1; i >= 0; i--) {
    if (!objs[i].alive) {
      num_dead_objs++;
      for (int j = i+1; j < *num_objs; j++) {
        objs[j-1] = objs[j];
      }
    }
  }

  *num_objs -= num_dead_objs;
}

bool checkObjCollision(GameObj *obj1, GameObj *obj2, Polygon *collision) {
  return checkCollision(
    &obj1->polygon, obj1->position,
    &obj2->polygon, obj2->position,
    collision
  );
}

Game *gameLogic(Game *game, uint16_t keys) {
  cleanDeadObjs(game->astroids, &game->num_astroids);
  
  shipGameLogic(game->ship, game->friction, keys);
  if (game->frame % 30 == 19 && game->num_astroids < game->max_num_astroids) {
    spawnAstroid(game);
  }

  for (int i = 0; i < game->num_astroids; i++) {
    astroidGameLogic(&game->astroids[i]);
    if (checkObjCollision(&game->astroids[i], game->ship)) {
      game->astroids[i].alive = false;
      printf("Collision with astroid %d!\n", i);
    }
  }
  
  game->frame++;
  return game;
}

void renderGameObj(GameObj *obj, matrix trans) {
  Polygon trans_poly = transform(&obj->polygon, trans);
  Color rect_color = make_vec(0.2, 0.2, 0.2);
  Polygon rect = boundingBox(&trans_poly);
  
  renderPolygon(&rect, obj->position, rect_color);
  renderPolygon(&trans_poly, obj->position, obj->color);
}

Game *gameRender(Game *game) {
  matrix rotate = rotation_matrix_2d(game->ship->rotation);
 
  renderGameObj(game->ship, rotate);
  for (int i = 0; i < game->num_astroids; i++) {
    GameObj *astro = &game->astroids[i];
    matrix rotate = rotation_matrix_2d(game->frame + 2*astro->rotation);
    if (astro->alive)
      renderGameObj(astro, rotate);
  }

  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

