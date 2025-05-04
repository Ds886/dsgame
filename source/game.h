#ifndef GAME_H
#define GAME_H

typedef struct game_t {
  int frame;
} Game;

Game *gameStart(Game *game);
Game *gameLogic(Game *game);
Game *gameRender(Game *game);
Game *gameEnd(Game *game);

#endif
