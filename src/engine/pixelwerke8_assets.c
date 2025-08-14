#include "pixelwerke8_assets.h"

AssetStatus SpriteInit(Sprite* sprite, AssetAddress start_address, uint8_t type){
	sprite->address = start_address;
	sprite->velocity = 0x00;
	sprite->sprite_flags = 0x04;
	sprite->animation = type;
	
	return ASSET_STATUS_OK;
}
