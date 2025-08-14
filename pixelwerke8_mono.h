#ifndef PIXELWERKE8_MONO_H
#define PIXELWERKE8_MONO_H

#include <stdint.h>
#include "pic18f16q41_system.h"
#include "st7789.h"
#include "pixelwerke8_assets.h"
#include "game_art.h"

#define PIXEL_SIZE 2
#define PIXEL_SQUARE (PIXEL_SIZE * PIXEL_SIZE)
#define MAX_PIXELS 64

#define TILE_SIZE 2

#define SCREEN_RES_X 240
#define SCREEN_RES_Y 240

#define TILE_PIXEL_CONV_X (((SCREEN_RES_X / PIXEL_SIZE) / TILE_SIZE) - 1)
#define TILE_PIXEL_CONV_Y (((SCREEN_RES_Y / PIXEL_SIZE) / TILE_SIZE) - 1)

#define CHAR_ARRAY_SIZE 5 * 5 * PIXEL_SQUARE * 2

#define MAX_LINES (SCREEN_RES_X / (6 * PIXEL_SIZE))
#define MAX_LINE_LENGTH (SCREEN_RES_X / (5 * PIXEL_SIZE))

#define RENDER_OK 0
#define RENDER_INVALID_INPUT 1
#define RENDER_DOWNSTREAM_ERROR 32
#define RENDER_FAILED_OP 64
#define RENDER_UNKNOWN_ERROR 128

extern const uint8_t DEFAULT_COLORS[4];

extern uint8_t on_color_lo;
extern uint8_t on_color_hi;
extern uint8_t off_color_lo;
extern uint8_t off_color_hi;

typedef uint8_t RenderStatus;

RenderStatus PW8MonoInit(uint8_t* game_colors);
RenderStatus PW8MonoDrawPixel(uint16_t address);
RenderStatus PW8MonoDrawTile(Tile* tile, uint8_t* art);
RenderStatus PW8MonoDrawSprite(Sprite* sprite);
RenderStatus PW8MonoClearSprite(Sprite* sprite);
RenderStatus PW8MonoWriteChar(uint8_t character, uint16_t address);
RenderStatus PW8MonoWriteString(uint8_t* string, uint8_t length, uint16_t address);
RenderStatus PW8MonoEraseChar(uint16_t address);
RenderStatus PW8MonoEraseString(uint16_t address, uint8_t length);
RenderStatus PW8MonoClearScreen(void);

#endif