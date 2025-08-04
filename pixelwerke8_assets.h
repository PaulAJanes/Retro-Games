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
	//An address, assumes a square grid
	AssetAddress address;
	
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

//Some unused structs to keep in mind for later assets
//Subject to change as-needed
typedef struct{
	
	AssetAddress address;
	
	uint8_t size;
	
} Tile;

typedef struct{
	
	uint8_t layer_flags;
	
} Layer;

typedef struct{
	
	Tile* tiles;
	
	Layer* layers;
	
	uint8_t size;
	
} Region;
//End unused assets

AssetStatus SpriteInit(Sprite* sprite, AssetAddress start_address, uint8_t type);

#endif