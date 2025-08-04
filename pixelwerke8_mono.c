#include "pixelwerke8_mono.h"
#include "game_art.h"

static const uint8_t COLORS[4] = {OFF_COLOR_HI, ON_COLOR_HI, OFF_COLOR_LO, ON_COLOR_LO};

//Function to draw a single monochrome pixel
//Takes in address of pixel
//Parses address into x and y, validating for inputs in range
//Returns RenderStatus
RenderStatus PW8MonoDrawPixel(uint16_t address){
	
	//Parse address into x and y
	uint8_t x = (address >> 8) & 0x00FF;
	uint8_t y = address & 0x00FF;
	
	//Validate address in range
	if(x > 239 || y > 239){
		return RENDER_INVALID_INPUT;
	}
	
	//Art array for writing pixel
	//A logical "pixel" is a square of PIXEL_SIZE x PIXEL_SIZE
	// << 1 because 2 bytes per pixel
	uint8_t turn_on[(PIXEL_SIZE * PIXEL_SIZE) << 1];
	
	//Fill array with "on" color
	for(uint8_t i = 0; i < ((PIXEL_SIZE * PIXEL_SIZE) << 1); i += 2){
		turn_on[i] = ON_COLOR_HI;
		turn_on[i + 1] = ON_COLOR_LO;
	}
	
	//Send data to driver for drawing and get status
	//All OK statuses that I've written or will write are 0
	//Relatively safe to assume that's true always in this context
	if(St7789Draw(x, x + PIXEL_SIZE - 1, y, y + PIXEL_SIZE - 1, turn_on, ((PIXEL_SIZE * PIXEL_SIZE) << 1)) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR;
	}
	
	//Return OK
	return RENDER_OK;
}

//Function to draw a tile to the screen
//Takes in the tile and the artwork for that tile
//Validates tile size and address
//Parses monochrome art and converts to 16-bit color
//Returns status
RenderStatus PW8MonoDrawTile(Tile* tile, uint8_t* art){
	
	//Calculate total pixels for validation
	uint8_t pixel_count = tile->size * tile->size * PIXEL_SQUARE;
	
	//Validate pixel count to ensure no more than 128 elements in the output array
	if(pixel_count > MAX_PIXELS){
		return RENDER_INVALID_INPUT;
	}
	
	//Parse address into x and y
	uint8_t x = (tile->address >> 8) & 0x00FF;
	uint8_t y = tile->address & 0x00FF;
	
	//Convert Tile address to Pixel address
	x = x * PIXEL_SIZE * tile->size;
	y = y * PIXEL_SIZE * tile->size;
	
	//Validate address in range
	if(x > 239 || y > 239){
		return RENDER_INVALID_INPUT;
	}
	
	//Initialize output array
	uint8_t two_byte_art[MAX_PIXELS << 1] = {0};
	
	//Calculate number of bits from size
	uint8_t bits = tile->size * tile->size;
	
	//Initialize some variables to walk the array
	uint8_t byte = 0;
	uint8_t bit = 0;
	uint8_t hi = 0;
	uint8_t lo = 0;
	uint8_t dex = 0;
	uint8_t col = (PIXEL_SIZE * tile->size) << 1;
	
	//Loop through input bits
	for(uint8_t i = 0; i < bits; i++){
		
		//Select current bit
		byte = art[i >> 3];
		bit = (byte >> (7 - (i & 7))) & 0x01;
		
		//Use bit to get hi and lo bytes of the color
		hi = COLORS[bit];
		lo = COLORS[bit | 0x02];
		
		//Select index of the hi byte of the top-left true pixel of the current logical pixel
		dex = ((i / tile->size) * tile->size * PIXEL_SQUARE * 2) + ((i % tile->size) * PIXEL_SIZE * 2);
		
		//Iterate through columns
		for(uint8_t j = 0; j < PIXEL_SIZE; j++){
			
			//Iterate through rows
			for(uint8_t k = 0; k < (PIXEL_SIZE << 1); k += 2){
				//Write hi and lo bytes of the color to the output array
				two_byte_art[dex + k] = hi;
				two_byte_art[dex + k + 1] = lo;
			}
			
			//Increment index to next column
			dex += col;
		}
	}
	
	//Send output array to screen and return status if error
	//Assumes that, since I wrote all of these, all OK status codes are 0
	//They are all 0
	if(St7789Draw(x, x + tile->size * PIXEL_SIZE - 1, y, y + tile->size * PIXEL_SIZE - 1, two_byte_art, pixel_count << 1) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR;
	}
	
	//Return OK
	return RENDER_OK;
}

//Function to draw a Sprite to the screen
//Takes in the Sprite
//Parses the Sprite data for address and artwork
//Fetches artwork from game files
//Draws artwork to the correct address on the screen using DrawTile
//DOES NOT handle wrapping
//DOES change Sprite address to valid bounds
//Returns status
RenderStatus PW8MonoDrawSprite(Sprite* sprite){
	
	//Parse art and size from Sprite
	uint8_t* art = GetArt(sprite->animation, sprite->sprite_flags);
	uint8_t height = (sprite->sprite_flags >> 6) & 0x03;
	uint8_t width = (sprite->sprite_flags >> 4) & 0x03;
	
	//Create Tile to draw
	Tile this_tile;
	this_tile.size = TILE_SIZE;
	
	//Placeholder for each Tile's artwork
	uint8_t this_art = 0;
	
	//Half-byte index for HI or LO
	//0 = HI, 1 = LO
	uint8_t half = 0;
	
	//Index counter
	uint8_t dex = 0;
	
	//Return status to check for errors
	//Initialized to an error code in case it never changes
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Parse address from Sprite data
	uint8_t x = (sprite->address >> 8) & 0x00FF;
	uint8_t y = sprite->address & 0x00FF;
	
	//Store initial Y address to allow top-to-bottom, left-to-right drawing
	uint8_t y_i = y;
	
	//Nested For loop to iterate through Tiles covered by Sprite
	for(uint8_t i = 0; i < width + 2; i++){
		
		//Validate x underflow
		if(x > 254){
			
			//If x underflows from 0, reset to 0
			x = 0;
			sprite->address &= 0x00FF;
			i--;
			continue;
		}
		
		//Validate x overflow
		if(x > TILE_PIXEL_CONV_X){
			
			//If x overflows max address, reset to max
			x = TILE_PIXEL_CONV_X;
			sprite->address &= 0x00FF;
			sprite->address |= (x >> 8);
			i--;
			continue;
		}
		
		for(uint8_t j = 0; j < height + 2; j++){
			
			if(y > 254){
				y = 0;
				sprite->address &= 0xFF00;
				j--;
				continue;
			}
			
			//Validate y in range
			if(y > TILE_PIXEL_CONV_Y){
				
				//If y out of range, start over at 0
				y = TILE_PIXEL_CONV_Y;
				sprite->address &= 0xFF00;
				sprite->address |= y;
				j--;
				continue;
			}
			
			//Reassemble address
			this_tile.address = ((x & 0x00FF) << 8) | (y & 0x00FF);
			
			//Switch to get HI or LO half of art byte
			//Return error if error
			switch(half){
				case 0:
					this_art = (art[dex] & 0xF0);
					half = 1;
					break;
				case 1:
					this_art = (art[dex] & 0x0F) << 4;
					half = 0;
					dex++;
					break;
				default:
					return RENDER_UNKNOWN_ERROR;
			}
			
			//Draw Tile, return error if error
			if((status = PW8MonoDrawTile(&this_tile, &this_art)) != RENDER_OK){
				return status;
			}
			
			y++;
		}
		
		x++;
		
		//Wrap back to top Y
		y = y_i;
	}
	
	//Return OK
	return RENDER_OK;
}

//Function to clear a Sprite's animation
//Takes in the Sprite
//Writes the OFF color to each pixel
//Like DrawSprite, this WILL overwrite Sprite address to keep it in bounds
//Returns status
RenderStatus PW8MonoClearSprite(Sprite* sprite){
	//Parse art and size from Sprite
	uint8_t art[2] = {OFF_COLOR_HI, OFF_COLOR_LO};
	uint8_t height = (sprite->sprite_flags >> 6) & 0x03;
	uint8_t width = (sprite->sprite_flags >> 4) & 0x03;
	
	//Create Tile to draw
	Tile this_tile;
	this_tile.size = TILE_SIZE;
	
	//Placeholder for each Tile's artwork
	uint8_t this_art = 0;
	
	//Half-byte index for HI or LO
	//0 = HI, 1 = LO
	uint8_t half = 0;
	
	//Index counter
	uint8_t dex = 0;
	
	//Return status to check for errors
	//Initialized to an error code in case it never changes
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Parse address from Sprite data
	uint8_t x = (sprite->address >> 8) & 0x00FF;
	uint8_t y = sprite->address & 0x00FF;
	
	//Store initial Y address to allow top-to-bottom, left-to-right drawing
	uint8_t y_i = y;
	
	//Nested For loop to iterate through Tiles covered by Sprite
	for(uint8_t i = 0; i < width + 2; i++){
		
		//Validate x underflow
		if(x > 254){
			
			//If x underflows from 0, reset to 0
			x = 0;
			sprite->address &= 0x00FF;
			i--;
			continue;
		}
		
		//Validate x overflow
		if(x > TILE_PIXEL_CONV_X){
			
			//If x overflows max address, reset to max
			x = TILE_PIXEL_CONV_X;
			sprite->address &= 0x00FF;
			sprite->address |= (x >> 8);
			i--;
			continue;
		}
		
		for(uint8_t j = 0; j < height + 2; j++){
			
			if(y > 254){
				y = 0;
				sprite->address &= 0xFF00;
				j--;
				continue;
			}
			
			//Validate y in range
			if(y > TILE_PIXEL_CONV_Y){
				
				//If y out of range, start over at 0
				y = TILE_PIXEL_CONV_Y;
				sprite->address &= 0xFF00;
				sprite->address |= y;
				j--;
				continue;
			}
			
			//Reassemble address
			this_tile.address = ((x & 0x00FF) << 8) | (y & 0x00FF);
			
			//Switch to get HI or LO half of art byte
			//Return error if error
			switch(half){
				case 0:
					this_art = (art[dex & 0x01] & 0xF0);
					half = 1;
					break;
				case 1:
					this_art = (art[dex & 0x01] & 0x0F) << 4;
					half = 0;
					dex++;
					break;
				default:
					return RENDER_UNKNOWN_ERROR;
			}
			
			//Draw Tile, return error if error
			if((status = PW8MonoDrawTile(&this_tile, &this_art)) != RENDER_OK){
				return status;
			}
			
			y++;
		}
		
		x++;
		
		//Wrap back to top Y
		y = y_i;
	}
	
	//Return OK
	return RENDER_OK;
}

//Not yet implemented
RenderStatus PW8MonoDrawRegion(Region* region, uint8_t* art){
	return RENDER_UNKNOWN_ERROR;
}