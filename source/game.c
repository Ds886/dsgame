#include "game.h"

#include <nds.h>

#define ELAPSED(a) ELAPSED_BETWEEN(a, frame)

static uint32_t frame = 0;

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
    float rotation, Color color, bool collidable
) {
    GameObj ret;

    ret.polygon = poly;
    ret.position = pos;
    ret.velocity = velocity;
    ret.rotation = rotation;
    ret.color = color;
    ret.alive = true;
    ret.collidable = collidable;
    ret.state_time = frame;
    ret.state = OBJ_STATE_BORN;

    return ret;
}

Astroid *findAstroidToReuse(Astroid *astros, int len_astros) {
  Astroid *astro;
  for (int i = 0; i< len_astros; i++) {
    astro = &astros[i];
    if (!astro->obj.alive)
      return astro;
  }

  return NULL;
}

Shoot *findShootToReuse(Shoot *shoots, int len_shoots) {
  Shoot *shoot;
  for (int i = 0; i< len_shoots; i++) {
    shoot = &shoots[i];
    if (!shoot->obj.alive)
      return shoot;
  }

  return NULL;
}

bool objChangeState(GameObj *obj, enum obj_state new_state, int wait) {
  if(ELAPSED(obj->state_time) >= wait) {
    obj->state = new_state;
    obj->state_time = frame;
    obj->collidable = (new_state == OBJ_STATE_NORMAL);
    return true;
  }

  return false;
}

Ship newShip(
    vec2 pos, float width, float height, float accel,
    float rotation_speed, float max_velocity,
    Shoot *shoots, int max_num_shoots,
    int initial_shoot_freq, int lives, Color color
) {
  Ship ship;
  Polygon poly = isoscelesTriangleCentered(PLAYER_WIDTH, PLAYER_HEIGHT);

  ship.acceleration = accel;
  ship.rotation_speed = rotation_speed;
  ship.max_velocity = max_velocity;
  ship.shoots = shoots;
  ship.max_num_shoots = max_num_shoots;
  ship.shoot_freq = initial_shoot_freq;
  ship.lives = lives;
  ship.obj = newGameObj(poly, pos, 0, 0, color, false);

  for (int i=0; i<max_num_shoots; i++)
    objChangeState(&shoots[i].obj, OBJ_STATE_DEAD, 0);

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
    int astroid_num_stages,
    float astroid_initial_size,
    float astroid_velocity,
    float friction,
    float player_accel,
    float player_rotation_speed,
    float player_max_velocity,
    int player_lives,
    Color player_color) 
{
  game->frame = 0;
  game->keys = 0;
  game->friction = friction;
  game->ship = ship;
  game->astroids = astroids;
  game->max_num_astroids = max_num_astroids;
  game->astroid_size = astroid_initial_size;
  game->astroid_velocity = astroid_velocity;
  game->astroid_num_stages = astroid_num_stages;
  
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
        player_lives,
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
  Shoot *shoot = findShootToReuse(ship->shoots, ship->max_num_shoots);
  if (!shoot)
    return;
  
  float rot = ship->obj.rotation;
  Polygon line = newLine(MAKE_VEC2(SHOOT_SIZE, 0));
  line = transform(&line, rotation_matrix_2d(rot-90));

  shoot->shooter = ship;
  shoot->obj = newGameObj(
    line, ship->obj.position, 10,
    rot, ship->obj.color, true
  );
}

void shipGameLogic(Ship *ship, float gameFriction, uint16_t keys, uint16_t pressed_keys) {
  switch(ship->obj.state) {
  case OBJ_STATE_NORMAL:
    if (!ship->obj.alive) {
      objChangeState(&ship->obj, OBJ_STATE_DYING, 0);

      return;
    }
  
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
      spawnShoot(ship);
    }

    if (ABS(ship->obj.velocity) > 0.1) {  
      s16 bin_rotation = degreesToAngle(-ship->obj.rotation);
      float cos = fixedToFloat(cosLerp(bin_rotation), 12);
      float sin = fixedToFloat(sinLerp(bin_rotation), 12);
      X(ship->obj.position) -= ship->obj.velocity * sin;
      Y(ship->obj.position) -= ship->obj.velocity * cos;
    }

    for (int i=0; i<ship->max_num_shoots; i++) {
      Shoot *sh = &ship->shoots[i];
      if (!sh->obj.alive)
        continue;
      s16 bin_rotation = degreesToAngle(-sh->obj.rotation);
      float cos = fixedToFloat(cosLerp(bin_rotation), 12);
      float sin = fixedToFloat(sinLerp(bin_rotation), 12);
      X(sh->obj.position) -= sh->obj.velocity * sin;
      Y(sh->obj.position) -= sh->obj.velocity * cos;

    
      if (OUT_OF_BOUNDS(sh->obj.position, SHOOT_SIZE)) {
        sh->obj.alive = false;
      }
    }

    crossScreen(&ship->obj.position);
    break;
  case OBJ_STATE_BORN:
    objChangeState(&ship->obj, OBJ_STATE_NORMAL, SHIP_ANIMATION_TIME);
    break;
  case OBJ_STATE_DYING:
    if(ship->lives > 0)
      objChangeState(&ship->obj, OBJ_STATE_BORN, SHIP_ANIMATION_TIME);
    else
      objChangeState(&ship->obj, OBJ_STATE_DEAD, SHIP_ANIMATION_TIME);
    break;
  case OBJ_STATE_DEAD:
    printf("Game Over\n");
    break;
  default:
    printf("where's my ship?\n");
    break;
  }
}

bool astroidGameLogic(Astroid *astro) {
  switch(astro->obj.state) {
  case OBJ_STATE_NORMAL:
    if (!astro->obj.alive)
      return false;

    s16 bin_rotation = degreesToAngle(-astro->obj.rotation);
    float cos = fixedToFloat(cosLerp(bin_rotation), 12);
    float sin = fixedToFloat(sinLerp(bin_rotation), 12);

    if (ABS(astro->obj.velocity) > 0.1) {
      X(astro->obj.position) += astro->obj.velocity * sin;
      Y(astro->obj.position) += astro->obj.velocity * cos;
    }
  
    if (OUT_OF_BOUNDS(astro->obj.position, 40)) {
      astro->obj.alive = false;
    }

    matrix m = rotation_matrix_2d(ASTROID_ANIMATION_SPEED);
    astro->obj.polygon = transform(&astro->obj.polygon, m);
    break;
  case OBJ_STATE_BORN:
    objChangeState(&astro->obj, OBJ_STATE_NORMAL, 0);
    break;
  default:
    printf("where's my astroid?\n");
    break;
  }

  return astro->obj.alive;
}

Astroid *spawnAstroid(Game *game, int stage, float scale, vector pos, float rot) {
  Astroid *astro = findAstroidToReuse(game->astroids, game->max_num_astroids);
  if (!astro)
    return NULL;

  float scale_fact = 1;
  for (int i = 0; i<stage; i++)
    scale_fact *= scale;

  Polygon poly = almostRegularPolygon(ASTRO_NUM_VERTICES, game->astroid_size, 0);
  poly = transform(&poly, mat_scaling(scale_fact));
  astro->obj = newGameObj(
    poly, pos, game->astroid_velocity,
    rot + 90, ASTROID_COLOR, true);

  centralizePolygon(&astro->obj.polygon);

  astro->stage = stage;
  return astro;
}

void splitAstroid(Game *game, Astroid *astro, float scale, int num_partitions) {
  float rot;

  for (int i = 0; i < num_partitions; i++) {
    rot = (float)(random() % 30);
    spawnAstroid(game, astro->stage+1, scale, astro->obj.position, astro->obj.rotation + rot);
  }

  astro->obj.alive = false;
}

void spawnFirstStageAstroid(Game *game) {
  vector pos;
  float rot = (float)(random() % 360);
  s16 bin_rotation = degreesToAngle(rot);
  float cos = fixedToFloat(cosLerp(bin_rotation), 12);
  float sin = fixedToFloat(sinLerp(bin_rotation), 12);
  pos = MAKE_VEC2(GAME_SCREEN_WIDTH/2, GAME_SCREEN_HEIGHT/2);
  float fact = game->astroid_size + GAME_SCREEN_WIDTH / 2;
  pos = vec_add(pos, MAKE_VEC2(cos * fact, sin * fact));
  spawnAstroid(game, 0, 1, pos, rot);
}

bool checkObjCollision(GameObj *obj1, GameObj *obj2, Polygon *collision) {
  return obj1->collidable && obj2->collidable
    && checkCollision(
      &obj1->polygon, obj1->position,
      &obj2->polygon, obj2->position,
      collision
    );
}

void respawnShip(Game *game) {
  Ship *ship = game->ship;
  float x,y;
  vector pos;

  bool good_place;
  int tries = 0;
  do {
    good_place = true;
    tries++;
    x = random() % GAME_SCREEN_WIDTH;
    y = random() % GAME_SCREEN_HEIGHT;
    pos = MAKE_VEC2(x, y);

    for (int i = 0; i < game->max_num_astroids; i++)
      if (game->astroids[i].obj.alive
          && checkObjCollision(&game->ship->obj, &game->astroids[i].obj, NULL)) {
        good_place = false;
        break;
      }
  } while(!good_place && tries < 20);

  objChangeState(&ship->obj, OBJ_STATE_BORN, 0);
  ship->lives--;
  ship->obj = newGameObj(ship->obj.polygon, pos, 0, ship->obj.rotation, ship->obj.color, false);
}

Game *gameLogic(Game *game, uint16_t keys) {
  shipGameLogic(game->ship, game->friction, keys, PRESSED_KEYS(game, keys));
  if (game->ship->obj.state == OBJ_STATE_BORN && !ELAPSED(game->ship->obj.state_time))
      respawnShip(game);

  if (game->frame % 50 == 19) {
    spawnFirstStageAstroid(game);
  }

  for (int i = 0; i < game->max_num_astroids; i++) {
    if (game->astroids[i].obj.alive)
      astroidGameLogic(&game->astroids[i]);

    Astroid *astro = &game->astroids[i];
    if (!astro->obj.alive)
      continue;

    if (checkObjCollision(&astro->obj, &game->ship->obj, NULL)) {
      game->ship->obj.alive = false;
    }

    for (int j=0 ;j < game->ship->max_num_shoots; j++){
      Shoot *shoot = &game->ship->shoots[j];
      if (!shoot->obj.alive)
        continue;
      if (checkObjCollision(&astro->obj, &shoot->obj, NULL)) {
        if (astro->stage < game->astroid_num_stages)
          splitAstroid(game, astro, ASTROID_SPLIT_SCALE, ASTROID_SPLIT_NUM_PARTITIONS);
        else
          astro->obj.alive = false;
        shoot->obj.alive = false;
      }
    }
  }

  game->keys = keys;
  game->frame++;
  frame++;

  return game;
}

void renderGameObjTransformed(GameObj *obj, matrix trans) {
  //TODO: changing color for debug purposes only!
  Color c = obj->color;
  if (!obj->collidable)
    c = make_vec(1, 1, 1);
  renderPolygonTransformed(&obj->polygon, obj->position, trans, c);
}

void renderGameObj(GameObj *obj) {
  renderGameObjTransformed(obj, mat_identity());
}

Game *gameRender(Game *game) { 
  matrix m;
  int elapsed;
  float sc;

  switch (game->ship->obj.state) {
  case OBJ_STATE_BORN:
    elapsed = ELAPSED(game->ship->obj.state_time);
    sc = (float)(4 * (SHIP_ANIMATION_TIME - elapsed) +  elapsed)/SHIP_ANIMATION_TIME;
    m = mat_scaling(sc);
    break;
  case OBJ_STATE_DYING:
    elapsed = ELAPSED(game->ship->obj.state_time);
    sc = (float)(20 * elapsed + SHIP_ANIMATION_TIME -  elapsed)/SHIP_ANIMATION_TIME;
    m = mat_scaling(sc);
    break;
  case OBJ_STATE_DEAD:
    //TODO: such a dirty hack!
    m = mat_scaling(1000);
    break;
  default:
    m = mat_identity();
    break;
  }
  
  renderGameObjTransformed(&game->ship->obj, m);
  
  for (int i = 0; i < game->ship->max_num_shoots; i++) {
    Shoot *shoot = &game->ship->shoots[i];
    if (shoot->obj.alive)
      renderGameObj(&shoot->obj);
  }

  for (int i = 0; i < game->max_num_astroids; i++) {
    GameObj *astro = &game->astroids[i].obj;
    if (astro->alive)
      renderGameObj(astro);
  }

  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

