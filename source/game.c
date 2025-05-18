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
    float rotation, Color color
) {
    GameObj ret;

    ret.polygon = poly;
    ret.position = pos;
    ret.velocity = velocity;
    ret.rotation = rotation;
    ret.color = color;
    ret.alive = true;

    return ret;
}


Ship newShip(
    vec2 pos, float width, float height, float accel,
    float rotation_speed, float max_velocity,
    Shoot *shoots, int max_num_shoots,
    int initial_shoot_freq, Color color
) {
  Ship ship;
  Polygon poly = isoscelesTriangleCentered(PLAYER_WIDTH, PLAYER_HEIGHT);

  ship.acceleration = accel;
  ship.rotation_speed = rotation_speed;
  ship.max_velocity = max_velocity;
  ship.shoots = shoots;
  ship.max_num_shoots = max_num_shoots;
  ship.shoot_freq = initial_shoot_freq;
  ship.obj = newGameObj(poly, pos, 0, 0, color);

  return ship;
}

Game *gameStart(
    Game *game,
    Ship *ship,
    Astroid *astroids,
    Shoot *shoots,
    int max_num_shoots,
    float initial_shoot_freq,
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
  game->keys = 0;
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

  for(int i = 0; i< max_num_astroids; i++)
    astroids[i].obj.alive = false;

  *ship = newShip (
        vecPosition,
        PLAYER_WIDTH, PLAYER_HEIGHT,
        player_accel,
        player_rotation_speed,
        player_max_velocity,
        shoots,
        max_num_shoots,
        initial_shoot_freq,
        player_color
    );

  
  return game;
}

void rotateGameObj(GameObj *obj, float degrees) {
  matrix rotate = rotation_matrix_2d(degrees);
  obj->polygon = transform(&obj->polygon, rotate);
  obj->rotation += degrees;
}

void spawnShoot(Ship *ship) {
  Shoot *shoot = &ship->shoots[ship->num_shoots++];
  float rot = ship->obj.rotation;
  Polygon line = newLine(MAKE_VEC2(SHOOT_SIZE, 0));
  line = transform(&line, rotation_matrix_2d(rot-90));

  shoot->shooter = ship;
  shoot->obj = newGameObj(
    line, ship->obj.position, 10,
    rot, ship->obj.color
  );
}

void shipGameLogic(Ship *ship, float gameFriction, uint16_t keys, uint16_t pressed_keys) {
  if (ship->obj.velocity > 0)
    ship->obj.velocity -= gameFriction;

  if (ship->obj.velocity < 0)
    ship->obj.velocity += gameFriction;

  if (keys & KEY_LEFT) {
    rotateGameObj(&ship->obj, -ship->rotation_speed);
  }

  if (keys & KEY_RIGHT) {
    rotateGameObj(&ship->obj, ship->rotation_speed);
  }

  if (keys & KEY_UP) {
    if (ship->obj.velocity < ship->max_velocity)
      ship->obj.velocity += ship->acceleration;
  }

  if (keys & KEY_DOWN) {
    if (ship->obj.velocity > -ship->max_velocity)
      ship->obj.velocity -= ship->acceleration;
  }

  if (pressed_keys & KEY_X) {
    if (ship->num_shoots < ship->max_num_shoots) {
      spawnShoot(ship);
    }
  }

  if (ABS(ship->obj.velocity) > 0.1) {  
    s16 bin_rotation = degreesToAngle(-ship->obj.rotation);
    float cos = fixedToFloat(cosLerp(bin_rotation), 12);
    float sin = fixedToFloat(sinLerp(bin_rotation), 12);
    X(ship->obj.position) -= ship->obj.velocity * sin;
    Y(ship->obj.position) -= ship->obj.velocity * cos;
  }

  for (int i=0; i<ship->num_shoots; i++) {
    Shoot *sh = &ship->shoots[i];
    if (!sh->obj.alive)
      continue;
    s16 bin_rotation = degreesToAngle(-sh->obj.rotation);
    float cos = fixedToFloat(cosLerp(bin_rotation), 12);
    float sin = fixedToFloat(sinLerp(bin_rotation), 12);
    X(sh->obj.position) -= sh->obj.velocity * sin;
    Y(sh->obj.position) -= sh->obj.velocity * cos;
  }

  crossScreen(&ship->obj.position);
}

void astroidGameLogic(Astroid *astro) {
  if (!astro->obj.alive)
    return;

  s16 bin_rotation = degreesToAngle(-astro->obj.rotation);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);

  if (ABS(astro->obj.velocity) > 0.1) {
    X(astro->obj.position) += astro->obj.velocity * sin;
    Y(astro->obj.position) += astro->obj.velocity * cos;
  }
  
  if (X(astro->obj.position) > GAME_SCREEN_WIDTH+40 || Y(astro->obj.position) > GAME_SCREEN_HEIGHT+40) {
    astro->obj.alive = false;
  }

  matrix m = rotation_matrix_2d(ASTROID_ANIMATION_SPEED);
  astro->obj.polygon = transform(&astro->obj.polygon, m);
}

void spawnAstroid(Game *game) {
  Astroid *astro = &game->astroids[game->num_astroids++];
  vector pos;

  float rot = (float)(random() % 360);
  
  s16 bin_rotation = degreesToAngle(rot);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);
  pos = MAKE_VEC2(GAME_SCREEN_WIDTH/2, GAME_SCREEN_HEIGHT/2);
  float fact = game->astroid_size + GAME_SCREEN_WIDTH / 2;
  pos = vec_add(pos, MAKE_VEC2(cos * fact, sin * fact));

  Polygon poly = almostRegularPolygon(ASTRO_NUM_VERTICES, game->astroid_size, 0);
  astro->obj = newGameObj(
    poly, pos, game->astroid_velocity,
    rot + 90, ASTROID_COLOR);

  centralizePolygon(&astro->obj.polygon);
}

void cleanDeadAstroids(Astroid *astros, int *num_astros) {
  int num_dead_astros = 0;
  for (int i = *num_astros-1; i >= 0; i--) {
    if (!astros[i].obj.alive) {
      num_dead_astros++;
      for (int j = i+1; j < *num_astros; j++) {
        astros[j-1] = astros[j];
      }
    }
  }

  *num_astros -= num_dead_astros;
}

bool checkObjCollision(GameObj *obj1, GameObj *obj2, Polygon *collision) {
  return checkCollision(
    &obj1->polygon, obj1->position,
    &obj2->polygon, obj2->position,
    collision
  );
}

Game *gameLogic(Game *game, uint16_t keys) {
  if (!game->ship->obj.alive) {
    printf("Game Over.\n");
    return game;
  }

  cleanDeadAstroids(game->astroids, &game->num_astroids);
  
  shipGameLogic(game->ship, game->friction, keys, PRESSED_KEYS(game, keys));
  if (game->frame % 300 == 19 && game->num_astroids < game->max_num_astroids) {
    spawnAstroid(game);
  }

  for (int i = 0; i < game->num_astroids; i++) {
    astroidGameLogic(&game->astroids[i]);
  }

  for (int i = 0; i < game->num_astroids; i++) {
    GameObj *astro = &game->astroids[i].obj;
    if (!astro->alive)
      continue;

    if (checkObjCollision(&game->astroids[i].obj, &game->ship->obj, NULL)) {
      game->ship->obj.alive = false;
    }

    for (int j=0 ;j < game->ship->num_shoots; j++){
      GameObj *shoot = &game->ship->shoots[j].obj;
      if (!shoot->alive)
        continue;
      if (checkObjCollision(astro, shoot, NULL)) {
        astro->alive = false;
        shoot->alive = false;
      }
    }
  }

  game->keys = keys;
  game->frame++;

  return game;
}

void renderGameObj(GameObj *obj) {
  renderPolygon(&obj->polygon, obj->position, obj->color);
}

Game *gameRender(Game *game) { 
  renderGameObj(&game->ship->obj);
  for (int i = 0; i < game->ship->num_shoots; i++) {
    Shoot *shoot = &game->ship->shoots[i];
    if (shoot->obj.alive)
      renderGameObj(&shoot->obj);
  }

  for (int i = 0; i < game->num_astroids; i++) {
    GameObj *astro = &game->astroids[i].obj;
    if (astro->alive)
      renderGameObj(astro);
  }

  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

