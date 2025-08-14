#ifndef PIXELWERKE8_ASSETS_H
#define PIXELWERKE8_ASSETS_H

#include <stdint.h>

#define ASSET_STATUS_OK 0
#define ASSET_INVALID_INPUT 1
#define ASSET_FAILED_OP 64
#define ASSET_UNKNOWN_ERROR 128

typedef uint8_t AssetStatus;

//Sets SpriteAddress to 16 bits
typedef uint16_t AssetAddress;

typedef struct{
	//An address
	//[31..16] are x
	//[15..0] are y
	AssetAddress address;
	
	//[7..3] are speed
	//[2..0] are direction
	uint8_t velocity;
	
	//[7..6] are height in tiles, 2-5
	//[5..4] are width in tiles, 2-5
	//[3] is mobile 1 vs fixed 0
	//[2] is alive 1 vs dead 0
	//[1] is in-play 1 vs staged 0
	//[0] is collisian enabled 1 vs disabled 0
	uint8_t sprite_flags;
	
	//Index of this sprite's animation
	uint8_t animation;
} Sprite;

//Some unused structs to keep in mind for later assets
//Subject to change as-needed
typedef struct{
	
	AssetAddress address;
	
	uint8_t size;
	
} Tile;

typedef struct{
	
	Tile* tiles;
	
	uint8_t flags;
} Region;
//End unused assets

AssetStatus SpriteInit(Sprite* sprite, AssetAddress start_address, uint8_t type);

#endif