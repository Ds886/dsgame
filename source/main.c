// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024

#include "nds/arm9/trig_lut.h"
#include "nds/arm9/videoGL.h"
#include "nds/input.h"
#include "nds/system.h"
#include <stdint.h>

#include <gl2d.h>
#include <nds.h>

#include "s8.h"
#include "s16.h"
#include "s64.h"

#include "linalg.h"
#include "polygon.h"
#include "game.h"

#define PLAYER_ACCEL 0.2
#define ROTATION_SPEED 3
#define PLAYER_MAX_VELOCITY 10
#define MAX_NUM_ASTROIDS 20
#define FRICTION 0.1
#define ASTROID_SIZE 40
#define ASTROID_VELOCITY 0.4
#define MAX_NUM_SHOOTS 10
#define INITIAL_SHOOT_FREQ 10
#define ASTROID_NUM_STAGES 3
#define PLAYER_LIVES 3

int main(int argc, char **argv)
{
    consoleDemoInit();
    videoSetMode(MODE_5_3D);
    glScreen2D();
    glEnable(GL_TEXTURE_2D);

    Game game;
    Color colorBase = make_vec(0.999, 0.1, 0.0);
    Ship poly;
    Astroid astroids[MAX_NUM_ASTROIDS];
    Shoot shoots[MAX_NUM_SHOOTS];


    char *mem_end = (char*)(shoots + MAX_NUM_SHOOTS);
    gameStart(
        &game,
        &poly,
        astroids,
        shoots,
        MAX_NUM_SHOOTS,
        INITIAL_SHOOT_FREQ,
        MAX_NUM_ASTROIDS,
        ASTROID_NUM_STAGES,
        ASTROID_SIZE,
        ASTROID_VELOCITY,
        FRICTION,
        PLAYER_ACCEL,
        ROTATION_SPEED,
        PLAYER_MAX_VELOCITY,
        PLAYER_LIVES,
        colorBase
    );

    while (1)
    {
        swiWaitForVBlank();
        consoleClear();
        scanKeys();
        uint16_t keys = keysHeld();

        gameLogic(&game, keys);

        glBegin2D();
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));

        gameRender(&game);

        printf("Lives: %d\n", poly.lives);
        printf("Score: %d\n", game.stats.score);
        printf("Astros destroied: %d\n", game.stats.num_astroids_destroied);
        printf("MEM: %p (%d)\n", &game, (int)(&game));
        printf("Memory required: %d\n", (int)(mem_end - (char*)&game));

        glEnd2D();
        glFlush(0);
    }

    gameEnd(&game);
    
    return 0;
}
