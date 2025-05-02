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

typedef struct vec3 color;

typedef struct obj2dData_t {
    struct vec2 position;
    struct vec2 velocity;
    float acceleration;
    float rotation;
    float rotation_speed;
} obj2dData;

struct spritestate {
    glImage *texture;
    obj2dData data;
};

struct polygonstate {
    Triangle triangle;
    obj2dData data;
};

struct vec3 vec3_mod(struct vec3 v1){
    while(v1.x > 1) v1.x -= 1;
    while(v1.y > 1) v1.y -= 1;
    while(v1.z > 1) v1.z -= 1;    

    while(v1.x < 0) v1.x += 1;
    while(v1.y < 0) v1.y += 1;
    while(v1.z < 0) v1.z += 1;

    return v1;
}

bool handleKeys(uint32_t keys, struct vec3 *color, obj2dData* spritedata){
        if (keys & KEY_START)
            return false;

        if (keys & KEY_X)
        {
            color->x = 255;
            color->y = 180;
            color->z = 120;
        }

        if (keys & KEY_B)
        {
            color->x = 20;
            color->y = 180;
            color->z = 255;
        }

        if (keys & KEY_Y)
        {
            color->x = 0;
            color->y = 180;
            color->z = 255;
        }

        if (keys & KEY_A)
        {
            color->x = 255;
            color->y = 180;
            color->z = 0;
        }
        
        s16 bin_rotation = degreesToAngle(spritedata->rotation);
        float cos = fixedToFloat(cosLerp(bin_rotation), 12);
        float sin = fixedToFloat(sinLerp(bin_rotation), 12);

        if (keys & KEY_UP)
        {
            spritedata->velocity.x += spritedata->acceleration * cos;
            spritedata->velocity.y += spritedata->acceleration * sin;
        }
        

        if (keys & KEY_DOWN)
        {
            spritedata->velocity.x -= spritedata->acceleration * cos;
            spritedata->velocity.y -= spritedata->acceleration * sin;
        }

        if (keys & KEY_LEFT)
        {
            spritedata->rotation -= spritedata->rotation_speed;
        }

        if (keys & KEY_RIGHT)
        {
            spritedata->rotation += spritedata->rotation_speed;
        }


    return true;
}

int loadTextures(glImage* texture, uint8_t texSize){
    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankE(VRAM_E_TEX_PALETTE);
    int tempTex ;
    const short unsigned int *pPal;
    const unsigned int *pBitmap;

    switch(texSize){
        case 8:
            pPal = s8Pal;
            pBitmap = s8Bitmap;
            break;
        case 16:
            pPal = s16Pal;
            pBitmap = s16Bitmap;
            break;
        case 64:
            pPal = s64Pal;
            pBitmap = s64Bitmap;
            break;
        default:
            return -1;
    }


    tempTex =
        glLoadTileSet(texture,
                      texSize, texSize,
                      texSize, texSize,
                      GL_RGB16,
                      texSize, texSize,
                      TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
                      16,
                      pPal,
                      pBitmap);

    if (tempTex < 0)
        printf("Failed to load texture:  %d\n", tempTex);

    return tempTex;
}

void clampColor(struct vec3 *color){
    color->x = (color->x < 0) ? 0 : (color->x > 1) ? 1 : color->x;
    color->y = (color->y < 0) ? 0 : (color->y > 1) ? 1 : color->y;
    color->z = (color->z < 0) ? 0 : (color->z > 1) ? 1 : color->z;
}


void InitColors(struct vec3* colorMod, uint8_t* nColorPhase){
    *nColorPhase = 1;
    colorMod->x = 0.0625;
    colorMod->y = 0.0312;
    colorMod->z = 0.1562;
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


struct polygonstate newTriangle(Triangle tri, struct vec2 pos, float accel, float rotation_speed) {
    struct polygonstate ret;

    ret.triangle = tri;
    ret.data.position = pos;
    ret.data.velocity.x = 0;
    ret.data.velocity.y = 0;
    ret.data.acceleration = accel;
    ret.data.rotation = 0;
    ret.data.rotation_speed = rotation_speed;

    return ret;
}

struct spritestate newSprite(glImage *texture, struct vec2 pos, float accel, float rotation_speed) {
    struct spritestate ret;

    ret.texture = texture;
    ret.data.position = pos;
    ret.data.velocity.x = 0;
    ret.data.velocity.y = 0;
    ret.data.acceleration = accel;
    ret.data.rotation = 0;
    ret.data.rotation_speed = rotation_speed;

    return ret;
}

void renderPolygon(struct polygonstate *poly, color col) {
    glTriangleFilled(
        poly->triangle.a.x,
        poly->triangle.a.y,
        poly->triangle.b.x,
        poly->triangle.b.y,
        poly->triangle.c.x,
        poly->triangle.c.y,
        COLOR_TO_15BIT(&col)
    );
} 

void renderSprite(struct spritestate *sprite) {
    glSpriteRotate(
        sprite->data.position.x - PLAYER_HALF_WIDTH,
        sprite->data.position.y - PLAYER_HALF_HEIGHT,
        degreesToAngle(sprite->data.rotation), 
        GL_FLIP_NONE,
        sprite->texture
    );
}

void set_in_position(struct polygonstate *poly) {
  matrix m;
  float diff_x = poly->data.position.x - poly->triangle.a.x;
  float diff_y = poly->data.position.y - poly->triangle.a.y;
  translate_matrix_2d(m, diff_x, diff_y);
  transform(&poly->triangle, m);
}


int main(int argc, char **argv)
{
    consoleDemoInit();
    struct vec2 vecPosition = {GAME_SCREEN_WIDTH / 2 - PLAYER_HALF_WIDTH, GAME_SCREEN_HEIGHT / 2 - PLAYER_HALF_HEIGHT};

    videoSetMode(MODE_5_3D);

    glScreen2D();
    glEnable(GL_TEXTURE_2D);

    glImage texture;

    loadTextures(&texture, 16);

    uint16_t nColorCountChange = 0;
    color colorBase = {0.468, 0.375, 0.406};
    color colorMod;
    color currColor = {0, 0, 0};
    uint8_t nColorPhase = 0;
    InitColors(&colorMod, &nColorPhase);

    Triangle tri = isoscelesTriangle(10, 16);

    struct polygonstate poly = newTriangle (
        tri,
        vecPosition,
        PLAYER_ACCEL,
        ROTATION_SPEED
    );

    while (1)
    {
        swiWaitForVBlank();

        consoleClear();

        // Print some controls
        printf("START:  Exit to loader\n");
        printf("r:%f,g:%f,b:%f,count:%d\n", currColor.x, currColor.y, currColor.z,nColorCountChange);
        #ifdef DEBUG_MODE
        printf("Player X: %f\n", poly.data.velocity.x);
        printf("Player Y: %f\n", poly.data.velocity.y);
        printf("Precieved player Y: %f, max = %d\n", poly.data.velocity.y - PLAYER_HALF_HEIGHT, GAME_SCREEN_HEIGHT -2);
        printf("\n");
        #endif

        if (nColorPhase > MAX_COLOR_PHASE)
        {
            currColor = vec3_mul(colorMod, nColorCountChange);
            currColor = vec3_mod(currColor);
            currColor = vec3_add(currColor, colorBase);
            clampColor(&currColor);

            nColorPhase = 0;
            nColorCountChange++;
        }
        nColorPhase++;
 
        scanKeys();

        uint16_t keys = keysHeld();
        handleKeys(keys, &currColor, &poly.data);

        glBegin2D();
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));
        glColor(COLOR_TO_15BIT(&currColor));

        matrix m;
        set_in_position(&poly);
        transform(&poly.triangle, m);
        renderPolygon(&poly, currColor);

        glEnd2D();

        glFlush(0);
        poly.data.position = vec2_add(poly.data.position, poly.data.velocity);
        crossScreen(&poly.data.position);
    }

    return 0;
}
