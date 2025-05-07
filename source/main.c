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

#define GAME_SCREEN_WIDTH 256
#define GAME_SCREEN_HEIGHT 192
#define GAME_SCREEN_BOUNDS 2
#define PLAYER_ACCEL 0.08f
#define ROTATION_SPEED 4

bool handleKeys(uint32_t keys, vector *color, GameObj* spritedata){
    //     if (keys & KEY_START)
    //         return false;

    //     if (keys & KEY_X)
    //     {
    //         color->x = 255;
    //         color->y = 180;
    //         color->z = 120;
    //     }

    //     if (keys & KEY_B)
    //     {
    //         color->x = 20;
    //         color->y = 180;
    //         color->z = 255;
    //     }

    //     if (keys & KEY_Y)
    //     {
    //         color->x = 0;
    //         color->y = 180;
    //         color->z = 255;
    //     }

    //     if (keys & KEY_A)
    //     {
    //         color->x = 255;
    //         color->y = 180;
    //         color->z = 0;
    //     }
        
    //     s16 bin_rotation = degreesToAngle(spritedata->rotation);
    //     float cos = fixedToFloat(cosLerp(bin_rotation), 12);
    //     float sin = fixedToFloat(sinLerp(bin_rotation), 12);

    //     if (keys & KEY_UP)
    //     {
    //         spritedata->velocity.x += spritedata->acceleration * cos;
    //         spritedata->velocity.y += spritedata->acceleration * sin;
    //     }
        

    //     if (keys & KEY_DOWN)
    //     {
    //         spritedata->velocity.x -= spritedata->acceleration * cos;
    //         spritedata->velocity.y -= spritedata->acceleration * sin;
    //     }

    //     if (keys & KEY_LEFT)
    //     {
    //         spritedata->rotation -= spritedata->rotation_speed;
    //     }

    //     if (keys & KEY_RIGHT)
    //     {
    //         spritedata->rotation += spritedata->rotation_speed;
    //     }


    return true;
}

int main(int argc, char **argv)
{
    consoleDemoInit();
    videoSetMode(MODE_5_3D);
    glScreen2D();
    glEnable(GL_TEXTURE_2D);

    Game game;
    Color colorBase = make_vec(0.999, 0.1, 0.0);
    GameObj poly;

    gameStart(
        &game,
        &poly,
        PLAYER_ACCEL,
        ROTATION_SPEED,
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

        glEnd2D();
        glFlush(0);
    }

    gameEnd(&game);
    
    return 0;
}
