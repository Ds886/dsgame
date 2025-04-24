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

#define GAME_SCREEN_WIDTH 256
#define GAME_SCREEN_HEIGHT 192
#define GAME_SCREEN_BOUNDS 2

#define MAX_COLOR_PHASE 2
#define MAX_COLOR_BITS 32

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_HALF_WIDTH (PLAYER_WIDTH / 2)
#define PLAYER_HALF_HEIGHT (PLAYER_HEIGHT / 2)
#define PLAYER_ACCEL 0.08f

#define ROTATION_SPEED 4

#define DEBUG_MODE 1

struct vec2 {
    float x;
    float y;
};

struct spritestate {
    glImage *texture;
    struct vec2 position;
    struct vec2 velocity;
    float acceleration;
    float rotation;
    float rotation_speed;
};

struct vec2 vec2_mul(struct vec2 v1, int scalar){
    struct vec2 temp = {v1.x * scalar,
                        v1.y * scalar};
    return temp;
}

struct vec2 vec2_add(struct vec2 v1, struct vec2 v2){
    struct vec2 temp = {v1.x + v2.x, v1.y + v2.y};
    return temp;
}

struct vec2 vec2_sub(struct vec2 v1, struct vec2 v2){
    struct vec2 temp = {v1.x - v2.x, v1.y - v2.y};
    return temp;
}

struct vec2 vec2_div(struct vec2 v1, int scalar){
    struct vec2 temp = {v1.x / scalar,
                        v1.y / scalar};
    return temp;
}

struct vec3 {
    uint8_t x;
    uint8_t y;
    uint8_t z;
};


struct vec3 vec3_mul(struct vec3 v1, uint32_t scalar){
    struct vec3 temp = {v1.x * scalar,
                        v1.y * scalar,
                        v1.z * scalar};
    return temp;
}

struct vec3 vec3_mod(struct vec3 v1, uint32_t scalar){
    struct vec3 temp = {v1.x % scalar,
                        v1.y % scalar,
                        v1.z % scalar};
    return temp;
}

struct vec3 vec3_add(struct vec3 v1, struct vec3 v2){
    struct vec3 temp = {v1.x + v2.x,
                        v1.y + v2.y,
                        v1.z + v2.z};
    return temp;
}
struct vec3 vec3_sub(struct vec3 v1, struct vec3 v2){
    struct vec3 temp = {v1.x - v2.x,
                        v1.y - v2.y,
                        v1.z - v2.z};
    return temp;
}
struct vec3 vec3_div(struct vec3 v1, int scalar){
    struct vec3 temp = {v1.x / scalar,
                        v1.y / scalar,
                        v1.z / scalar};
    return temp;
}

bool handleKeys(uint32_t keys, struct vec3 *color, struct vec2* vecVelocity, s16* rotation){
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
        s16 bin_rotation = degreesToAngle(*rotation);
        float cos = fixedToFloat(cosLerp(bin_rotation), 12);
        float sin = fixedToFloat(sinLerp(bin_rotation), 12);

        if (keys & KEY_UP)
        {
            vecVelocity->x += PLAYER_ACCEL * cos;
            vecVelocity->y += PLAYER_ACCEL * sin;
        }
        

        if (keys & KEY_DOWN)
        {
            vecVelocity->x -= PLAYER_ACCEL * cos;
            vecVelocity->y -= PLAYER_ACCEL * sin;
        }

        if (keys & KEY_LEFT)
        {
            *rotation -= ROTATION_SPEED;
        }

        if (keys & KEY_RIGHT)
        {
            *rotation += ROTATION_SPEED;
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
    color->x = (color->x < 0) ? 0 : (color->x > 31) ? 31 : color->x;
    color->y = (color->y < 0) ? 0 : (color->y > 31) ? 31 : color->y;
    color->z = (color->z < 0) ? 0 : (color->z > 31) ? 31 : color->z;
}


void InitColors(struct vec3* colorMod, uint8_t* nColorPhase){
    *nColorPhase = 1;
    colorMod->x = 2;
    colorMod->y = 1;
    colorMod->z = 5;
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

struct spritestate newSprite(glImage *texture, struct vec2 pos, struct vec3 color, float accel, float rotation_speed) {
    struct spritestate ret;

    ret.texture = texture;
    ret.position = pos;
    ret.velocity.x = 0;
    ret.velocity.y = 0;
    ret.acceleration = accel;
    ret.rotation = 0;
    ret.rotation_speed = rotation_speed;

    return ret;
}

int main(int argc, char **argv)
{
    consoleDemoInit();
    struct vec2 vecVelocity = {0, 0};
    struct vec2 vecPosition = {GAME_SCREEN_WIDTH / 2 - PLAYER_HALF_WIDTH, GAME_SCREEN_HEIGHT / 2 - PLAYER_HALF_HEIGHT};
    s16 rotation = 0;

    videoSetMode(MODE_5_3D);

    glScreen2D();
    glEnable(GL_TEXTURE_2D);

    glImage texture[3];

    loadTextures(&texture[0], 8);
    loadTextures(&texture[1], 16);
    loadTextures(&texture[2], 64);
    uint16_t nColorCountChange = 0;
    struct vec3 colorBase = {15, 12, 13};
    struct vec3 colorMod;
    struct vec3 color = {0, 0, 0};
    uint8_t nColorPhase = 0;
    InitColors(&colorMod, &nColorPhase);
    while (1)
    {
        swiWaitForVBlank();

        consoleClear();

        // Print some controls
        printf("START:  Exit to loader\n");
        printf("r:%d,g:%d,b:%d,count:%d\n", color.x, color.y, color.z,nColorCountChange);
        #ifdef DEBUG_MODE
        printf("Player X: %f\n", vecVelocity.x);
        printf("Player Y: %f\n", vecVelocity.y);
        printf("Precieved player Y: %f, max = %d\n", vecVelocity.y - PLAYER_HALF_HEIGHT, GAME_SCREEN_HEIGHT -2);
        printf("\n");
        #endif

        if (nColorPhase > MAX_COLOR_PHASE)
        {
            color = vec3_mul(colorMod, nColorCountChange);
            color = vec3_mod(color, MAX_COLOR_BITS);
            color = vec3_add(color, colorBase);
            clampColor(&color);

            nColorPhase = 0;
            nColorCountChange++;
        }
        nColorPhase++;
 
        scanKeys();

        uint16_t keys = keysHeld();
        handleKeys(keys, &color, &vecVelocity, &rotation);

        glBegin2D();
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));
        glColor(RGB15(color.x, color.y, color.z));

        glSpriteRotate(vecPosition.x - PLAYER_HALF_WIDTH, vecPosition.y - PLAYER_HALF_HEIGHT, degreesToAngle(rotation), GL_FLIP_NONE, &texture[1]);

        glEnd2D();

        glFlush(0);
        vecPosition = vec2_add(vecPosition, vecVelocity);
        crossScreen(&vecPosition);
    }

    return 0;
}
