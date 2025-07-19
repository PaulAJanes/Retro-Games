#ifndef PIXELWERKE8_SPRITE_H
#define PIXELWERKE8_SPRITE_H

#include <stdint.h>

#define SPRITE_STATUS_OK 0
#define SPRITE_INVALID_INPUT 1
#define SPRITE_FAILED_OP 64
#define SPRITE_UNKNOWN_ERROR 128

typedef uint8_t SpriteStatus;

//Sets SpriteAddress to 16 bits
typedef uint16_t SpriteAddress;

typedef struct{
	//An address, assumes a square grid
	SpriteAddress address;
	
	//[0..2] are direction
	//[3..7] are speed
	uint8_t velocity;
	
	//[0] is collision
	//[1] is in-play vs staged
	//[2] is alive vs dead
	//[3] is mobile vs fixed
	//[4..5] are width in tiles, 1-4
	//[6..7] are height in tiles, 1-4
	uint8_t sprite_flags;
	
	//Index of this sprite's animation
	uint8_t animation;
} Sprite;

SpriteStatus SpriteInit(Sprite* sprite, SpriteAddress start_address, uint8_t type);

#endif