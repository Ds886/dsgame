#ifndef GAME_H
#define GAME_H 1

#include "polygon.h"

enum entity_type {
  NONE, SPRITE, POLYGON_2D
};

typedef struct entity_t {
  enum entity_type type;
  void *data;
} Entity;

typedef struct gamestate_t {
  Entity *entities; 
} GameState;

typedef struct spritestate_t {
    glImage *texture;
    struct vec2 position;
    struct vec2 velocity;
    float acceleration;
    float rotation;
    float rotation_speed;
} SpriteState;

typedef struct polygonstate_t {
    Triangle triangle;
    struct vec2 velocity;
    float acceleration;
    float rotation_speed;
} PolygonState;

#endif
