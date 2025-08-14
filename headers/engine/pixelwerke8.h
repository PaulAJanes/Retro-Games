#ifndef PIXELWERKE8_H
#define PIXELWERKE8_H

#include <stdint.h>
#include "pixelwerke8_assets.h"
#include "pixelwerke8_mono.h"
#include "audio.h"

#define MAX_SPRITES_ENGINE 256

#define ENGINE_OK 0
#define ENGINE_INVALID_INPUT 1
#define ENGINE_MAX_SPRITES_EXCEEDED 2
#define ENGINE_COLLISION_DETECTED 16
#define ENGINE_DOWNSTREAM_ERROR 32
#define ENGINE_FAILED_OP 64
#define ENGINE_UNKNOWN_ERROR 128

//Engine status for function returns
typedef uint8_t EngineStatus;

//[7] is game over
//[6..1] are undefined or game-specific
//[0] is collision detected with player
typedef uint8_t GameFlags;

//Engine instance to set up for the game
typedef struct EngineInstance EngineInstance;

EngineStatus PWEngineInit(uint8_t sprites_needed, uint8_t flags, Sprite* sprite_array, uint8_t* notes, uint8_t* colors);
EngineStatus PWDetectCollisions(uint8_t start, uint16_t* colliders);
EngineStatus PWRepackSprites(uint8_t old_count, uint8_t* new_count);
EngineStatus PWMoveSprites(uint8_t sprite_count);
EngineStatus PWRedrawSprites(uint8_t sprite_count);
EngineStatus PWEraseSprite(Sprite* sprite);
EngineStatus PWPlayTone(uint8_t tone);
EngineStatus PWSilence(void);
EngineStatus PWDisplayMessage(uint8_t** message, uint8_t* str_lens, uint8_t length);
EngineInstance* PWEngineGetInstance(void);

#endif
