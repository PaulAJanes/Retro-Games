#ifndef PIXELWERKE8_H
#define PIXELWERKE8_H

#include <stdint.h>
#include "pixelwerke8_sprite.h"

#define MAX_SPRITES 255
#define ENGINE_OK 0
#define ENGINE_INVALID_INPUT 1
#define ENGINE_MAX_SPRITES_EXCEEDED 2
#define ENGINE_COLLISION_DETECTED 16
#define ENGINE_DOWNSTREAM_ERROR 32
#define ENGINE_FAILED_OP 64
#define ENGINE_UNKNOWN_ERROR 128

//Engine status for function returns
typedef uint8_t EngineStatus;

//[0] is collision detected with player
//[1..7] are undefined
typedef uint8_t GameFlags;

//Engine instance to set up for the game
typedef struct EngineInstance EngineInstance;

EngineStatus PWEngineInit(uint8_t sprites_needed, uint8_t flags, Sprite* sprite_array);
EngineStatus PWDetectCollisions(uint8_t start, uint16_t* colliders);
EngineStatus PWRepackSprites(uint8_t old_count, uint8_t* new_count);
EngineStatus PWMoveSprites(uint8_t sprite_count);
EngineInstance* PWEngineGetInstance(void);

#endif