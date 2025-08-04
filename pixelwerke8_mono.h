#ifndef PIXELWERKE8_MONO_H
#define PIXELWERKE8_MONO_H

#include <stdint.h>
#include "st7789.h"
#include "pixelwerke8_assets.h"

#define PIXEL_SIZE 2
#define PIXEL_SQUARE (PIXEL_SIZE * PIXEL_SIZE)
#define MAX_PIXELS 64

#define TILE_SIZE 2

#define SCREEN_RES_X 240
#define SCREEN_RES_Y 240

#define TILE_PIXEL_CONV_X (((SCREEN_RES_X / PIXEL_SIZE) / TILE_SIZE) - 1)
#define TILE_PIXEL_CONV_Y (((SCREEN_RES_Y / PIXEL_SIZE) / TILE_SIZE) - 1)

#define ON_COLOR_HI 0xFF
#define ON_COLOR_LO 0xFF
#define OFF_COLOR_HI 0x00
#define OFF_COLOR_LO 0x00

#define RENDER_OK 0
#define RENDER_INVALID_INPUT 1
#define RENDER_DOWNSTREAM_ERROR 32
#define RENDER_FAILED_OP 64
#define RENDER_UNKNOWN_ERROR 128

typedef uint8_t RenderStatus;

RenderStatus PW8MonoDrawPixel(uint16_t address);
RenderStatus PW8MonoDrawTile(Tile* tile, uint8_t* art);
RenderStatus PW8MonoDrawSprite(Sprite* sprite);
RenderStatus PW8MonoClearSprite(Sprite* sprite);
RenderStatus PW8MonoDrawRegion(Region* region, uint8_t* art);

#endif