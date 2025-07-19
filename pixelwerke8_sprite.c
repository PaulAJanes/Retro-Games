#include "pixelwerke8_sprite.h"

SpriteStatus SpriteInit(Sprite* sprite, SpriteAddress start_address, uint8_t type){
	*sprite.address = start_address;
	*sprite.velocity = 0x00;
	*sprite.sprite_flags = 0x04;
	*sprite.animation = type;
	
	return SPRITE_STATUS_OK;
}