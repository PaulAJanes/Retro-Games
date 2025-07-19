#include "pixelwerke8.h"

//Struct to store information relating to the running instance of the engine
struct EngineInstance{
	//Max sprites to be allowed in this game instance
	//Max max is defined in pixelwerke8.h
	uint8_t max_sprites;
	
	//[0..7] are undefined
	uint8_t engine_flags;
	
	//Array of all sprites in the game
	Sprite* sprites;
};

//Instantiate instance and flags
static EngineInstance instance;
static GameFlags flags;

//Function to initialize game instance
//Takes number of sprites, flag byte, and pointer to sprite array
//Returns EngineStatus
EngineStatus PWEngineInit(uint8_t sprites_needed, uint8_t flags, Sprite* sprite_array){
	
	instance.max_sprites = sprites_needed;
	instance.engine_flags = flags;
	instance.sprites = sprite_array;
	
	return ENGINE_OK;
}

//Function to detect collisions
//Takes starting index as input
//Takes pointer to colliders
//Modifies colliders parameter in place such that [15..8] is the first object's index, and [7..0] is the second
//Returns EngineStatus
//Access sprite array and checks for matching addresses of collidable objects
EngineStatus PWDetectCollisions(uint8_t start, uint16_t* colliders){
	
	if(start >= instance.max_sprites){
		return ENGINE_MAX_SPRITES_EXCEEDED | ENGINE_INVALID_INPUT;
	}
	
	if(colliders == NULL){
		return ENGINE_INVALID_INPUT;
	}
	
	//Cycle through sprites for the first object
	for(uint8_t i = start; i < instance.max_sprites - 1; i++){
		
		//Check if first object can collide
		if(!(instance.sprites[i].sprite_flags & 0x01)){
			continue;
		}
		
		//Cycle through sprites for the second object
		for(uint8_t j = i + 1; j < instance.max_sprites; j++){
			
			//Check if second object can collide
			if(!(instance.sprites[j].sprite_flags & 0x01)){
				continue;
			}
			
			//Check address x plus width and address y plus height vs address of other
			if(!(((instance.sprites[i].address & 0x00FF) + ((instance.sprites[i].sprite_flags & 0x30) >> 4) < (instance.sprites[j].address & 0x00FF)) ||
				((instance.sprites[j].address & 0x00FF) + ((instance.sprites[j].sprite_flags & 0x30) >> 4) < (instance.sprites[i].address & 0x00FF)) ||
				(((instance.sprites[i].address & 0xFF00) >> 8) + ((instance.sprites[i].sprite_flags & 0xC0) >> 6) < ((instance.sprites[j].address & 0xFF00) >> 8)) ||
				(((instance.sprites[j].address & 0xFF00) >> 8) + ((instance.sprites[j].sprite_flags & 0xC0) >> 6) < ((instance.sprites[i].address & 0xFF00) >> 8)))){
					
				//Set colliders to indices as described above
				*colliders = (i << 8) | j;
				
				//Return collision detected
				return ENGINE_COLLISION_DETECTED;
			}
				
		}
	}
	
	//Return OK if not found
	return ENGINE_OK;
}

//Function to repack sprites, elminating dead sprites
//Takes in total number of sprites and pointer to new total
//Walks through sprite array, moving live sprites to indices of dead sprites
//Modifies new_count to hold new count
//Returns EngineStatus
EngineStatus PWRepackSprites(uint8_t old_count, uint8_t* new_count){
	
	//Validate input
	if(old_count > instance.max_sprites){
		return ENGINE_INVALID_INPUT | ENGINE_MAX_SPRITES_EXCEEDED;
	}
	
	if(new_count == NULL){
		return ENGINE_INVALID_INPUT;
	}
	
	//Index to write next live sprite
	uint8_t write = 1;
	
	//Walk through sprites
	for(uint8_t read = 1; read < old_count; read++){
		
		//Check each sprite for life
		if(instance.sprites[read].sprite_flags & 0x04){
			
			//If alive and read and write are not equal
			if(write != read){
				
				//Copy sprite from dead index to live index
				instance.sprites[write] = instance.sprites[read];
			}
		}
		
		//Increment write index
		write++;
	}
	
	//Write = last live sprite + 1 = total sprites
	*new_count = write;
	
	//Return OK
	return ENGINE_OK;
}

//Function to update positions of sprites
//Takes in current count of sprites
//Returns EngineStatus
//Updates all sprites in the instance based on velocity
//Directions start at 000 N and move clockwise to 111 NW
EngineStatus PWMoveSprites(uint8_t sprite_count){
	
	//Validate input
	if(sprite_count > instance.max_sprites){
		return ENGINE_MAX_SPRITES_EXCEEDED | ENGINE_INVALID_INPUT;
	}
	
	//Create x and y placeholders
	uint8_t x = 0;
	uint8_t y = 0;
	
	//Iterate through all sprites
	for(uint8_t i = 0; i < sprite_count; i++){
		
		//Skip if immobile or dead
		if((instance.sprites[i].sprite_flags & 0x0C) != 0x0C){
			continue;
		}
		
		//Extract x and y data from address
		x = (instance.sprites[i].address & 0x00FF);
		y = (instance.sprites[i].address >> 8) & 0x00FF;
		
		//Extract direction from velocity
		switch(instance.sprites[i].velocity & 0x07){
			//000 -> N
			case 0:
				y--;
				break;
			//001 -> NE
			case 1:
				x++;
				y--;
				break;
			//010 -> E
			case 2:
				x++;
				break;
			//011 -> SE
			case 3:
				x++;
				y++;
				break;
			//100 -> S
			case 4:
				y++;
				break;
			//101 -> SW
			case 5:
				x--;
				y++;
				break;
			//110 -> W
			case 6:
				x--;
				break;
			//111 -> NW
			case 7:
				x--;
				y--;
				break;
			//Result of bitmask should be between 0 and 7 always
			//If not, something has gone horribly wrong, return error
			default:
				return ENGINE_UNKNOWN_ERROR;
				break;
		}
		
		//Repack address and save
		instance.sprites[i].address = ((uint16_t) y << 8) | x;
	}
	
	//Reached the end, return ok
	return ENGINE_OK;
}

//Function to return the running instance of the engine
//Takes no inputs
//Returns only a pointer to the running engine instance
EngineInstance* PWEngineGetInstance(void){
	return &instance;
}