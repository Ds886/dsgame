#include "game.h"

Game *gameStart(Game *game) {
  game->frame = 0;
  
  return game;
}

Game *gameLogic(Game *game) {
  game->frame++;

  return game;
}

Game *gameRender(Game *game) {
  return game;  
}

Game *gameEnd(Game *game) {
  return game;  
}

