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

#define MAX_COLOR_PHASE 2
#define MAX_COLOR_BITS 5

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_HALF_WIDTH (PLAYER_WIDTH / 2)
#define PLAYER_HALF_HEIGHT (PLAYER_HEIGHT / 2)
#define PLAYER_ACCEL 0.08f

#define ROTATION_SPEED 4

#define DEBUG_MODE 1

#define FLOAT_TO_5BITS(n) ((uint16_t)(((1 << MAX_COLOR_BITS)-1)*n))
#define COLOR_TO_15BIT(col) (RGB15(FLOAT_TO_5BITS((col)->x), FLOAT_TO_5BITS((col)->y), FLOAT_TO_5BITS((col)->z)))

bool handleKeys(uint32_t keys, struct vec3 *color, GameObj* spritedata){
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

void clampColor(struct vec3 *color){
    color->x = (color->x < 0) ? 0 : (color->x > 1) ? 1 : color->x;
    color->y = (color->y < 0) ? 0 : (color->y > 1) ? 1 : color->y;
    color->z = (color->z < 0) ? 0 : (color->z > 1) ? 1 : color->z;
}

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


void renderPolygon(GameObj *poly, Color color) {
    glTriangleFilled(
        poly->triangle.a.x,
        poly->triangle.a.y,
        poly->triangle.b.x,
        poly->triangle.b.y,
        poly->triangle.c.x,
        poly->triangle.c.y,
        COLOR_TO_15BIT(&color)
    );
} 

void printMatrix(matrix m) {
    for (int i = 0; i< MAT_SIZE; i++) {
        printf("[");
        for (int j = 0; j < MAT_SIZE; j++) {
            printf("%f ", m[i][j]);
        }
        printf("]\n");
    }
}


void set_in_position(GameObj *poly) {
  matrix m;
  float diff_x = poly->position.x - poly->triangle.a.x;
  float diff_y = poly->position.y - poly->triangle.a.y;
  translate_matrix_2d(m, diff_x, diff_y);
  transform(&poly->triangle, m);
}


int main(int argc, char **argv)
{
    consoleDemoInit();
    videoSetMode(MODE_5_3D);

    glScreen2D();
    glEnable(GL_TEXTURE_2D);

    Game game;

    Color colorBase = {0.468, 0.375, 0.406};

    
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

        gameLogic(&game);

        // Print some controls
        scanKeys();

        uint16_t keys = keysHeld();
        handleKeys(keys, &colorBase, &poly);

        gameRender(&game);

        glBegin2D();
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));
        glColor(COLOR_TO_15BIT(&colorBase));

        matrix m;
        set_in_position(&poly);
        printMatrix(m);
        
        transform(&poly.triangle, m);
        renderPolygon(&poly, colorBase);
        printf("aft:\n%f, %f\n%f, %f\n", poly.triangle.a.x, poly.triangle.a.y, poly.triangle.b.x, poly.triangle.b.y);

        glEnd2D();

        glFlush(0);
        poly.position = vec2_add(poly.position, poly.velocity);
        crossScreen(&poly.position);
    }

    gameEnd(&game);
    
    return 0;
}
