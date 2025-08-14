#include "pixelwerke8_mono.h"

//Colors to be used for rendering
//[0] is hi byte of background (off) color
//[1] is hi byte of foreground (on) color
//[2] is lo byte of background (off) color
//[3] is lo byte of foreground (on) color
static uint8_t colors[4];

//Default colors: black off, white on
const uint8_t DEFAULT_COLORS[4] = {0x00, 0xFF, 0x00, 0xFF};

//Function to initialize renderer
//Takes in color pallette in the format laid out above at colors[4]
//Sets color pallette to match input
//Inits SPI
//Inits ST7789
//Returns status
RenderStatus PW8MonoInit(uint8_t* game_colors){
	
	//Copy color pallette
	colors[0] = game_colors[0];
	colors[1] = game_colors[1];
	colors[2] = game_colors[2];
	colors[3] = game_colors[3];
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Attempt to init SPI, must be before ST7789 init
	//If error, return error
	if((status = Spi1TxInit()) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR | status;
	}
	
	//Attempt to init ST7789, must be after SPI init
	//If error, return error
	if((status = St7789Init()) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR | status;
	}
	
	//Return OK
	return RENDER_OK;
}

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
		turn_on[i] = colors[1];
		turn_on[i + 1] = colors[3];
	}
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Send data to driver for drawing and get status
	//All OK statuses that I've written or will write are 0
	//Relatively safe to assume that's true always in this context
	if((status = St7789Draw(x, x + PIXEL_SIZE - 1, y, y + PIXEL_SIZE - 1, turn_on, ((PIXEL_SIZE * PIXEL_SIZE) << 1))) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR | status;
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
		hi = colors[bit];
		lo = colors[bit | 0x02];
		
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
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Send output array to screen and return status if error
	//Assumes that, since I wrote all of these, all OK status codes are 0
	//They are all 0
	if((status = St7789Draw(x, x + tile->size * PIXEL_SIZE - 1, y, y + tile->size * PIXEL_SIZE - 1, two_byte_art, pixel_count << 1)) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR | status;
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
	uint8_t* art = GetSpriteArt(sprite->animation, sprite->sprite_flags);
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
	uint8_t art[2] = {colors[0], colors[2]};
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

//Function to write a character to the screen
//Takes in the character and the top-left logical pixel address
//Converts to true pixel address
//Fetches character artwork
//Calls DrawPixel to draw pixels
//Does NOT handle invalid addresses or wrapping in any way
//Returns status
RenderStatus PW8MonoWriteChar(uint8_t character, uint16_t address){
	
	//Adjust address by logical pixel size
	uint16_t pix_add = address * PIXEL_SIZE;
	
	//Parse address into x and y
	uint8_t pix_x = (pix_add >> 8) & 0x00FF;
	uint8_t pix_y = pix_add & 0x00FF;
	
	//Row and column counters to handle art properly
	uint8_t col_ctr = 0;
	uint8_t row_ctr = 0;
	
	//Fetch correct artwork
	const uint8_t* char_art = GetFont(character);
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Iterate through each byte in the art
	for(uint8_t i = 0; i < 3; i++){
		
		//Iterate through each bit in the byte
		for(uint8_t j = 0; j < 8; j++){
			
			//If bit is a 1, draw it
			//If bit is a 0, move on
			if(char_art[i] & (0x80 >> j)){
				
				//Calculate correct address
				pix_add = ((pix_x + col_ctr) << 8) | (pix_y + row_ctr);
				
				//Attempt to draw pixel, if error return error
				if((status = PW8MonoDrawPixel(pix_add)) != RENDER_OK){
					return status;
				}
			}
			
			//Increment row counter by logical pixel size
			row_ctr += PIXEL_SIZE;
			
			//If 5 pixels have been written already
			if(row_ctr > (PIXEL_SIZE * 4)){
				
				//Go back to row 0
				row_ctr = 0;
				
				//And increment column counter by logical pixel size
				col_ctr += PIXEL_SIZE;
			}
		}
	}
	
	//Return OK
	return RENDER_OK;
}

//Function to write a string to the screen
//Takes in the string, the string length, and the top-left logical address
//Adjusts address to write left to right
//Calls WriteChar for each character given
//Returns status
RenderStatus PW8MonoWriteString(uint8_t* string, uint8_t length, uint16_t address){
	
	//Parse address to shift for each character
	uint8_t x = (address >> 8) & 0x00FF;
	uint8_t y = address & 0x00FF;
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Iterate through each character
	for(uint8_t i = 0; i < length; i++){
		
		//Attempt to write character, if error return error
		if((status = PW8MonoWriteChar(string[i], ((x + 5 * i) << 8) | y)) != RENDER_OK){
			return status;
		}
	}
	
	//Return OK
	return RENDER_OK;
}

//Function to erase a character written to the screen
//Takes in the top-left address of the character
//Colors the 5x5 logical pixel character area to the OFF color
//Returns status
RenderStatus PW8MonoEraseChar(uint16_t address){
	
	//Adjust address from logical pixel to true pixel
	uint16_t pix_add = address * PIXEL_SIZE;
	
	//Parse address into x and y for starting positions
	uint8_t x_start = (pix_add >> 8) & 0x00FF;
	uint8_t y_start = pix_add & 0x00FF;
	
	//Get end positions for x and y using logical pixel size and char size, which is 5x5
	uint8_t x_end = x_start + (5 * PIXEL_SIZE) - 1;
	uint8_t y_end = y_start + (5 * PIXEL_SIZE) - 1;
	
	//Create array to hold clear artwork
	uint8_t art[CHAR_ARRAY_SIZE];
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Populate clear artwork with OFF color
	for(uint8_t i = 0; i < CHAR_ARRAY_SIZE; i += 2){
		art[i] = colors[0];
		art[i + 1] = colors[2];
	}
	
	//Attempt to draw OFF color to all the pixels in the char
	//If error, return error
	if((status = St7789Draw(x_start, x_end, y_start, y_end, art, CHAR_ARRAY_SIZE)) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR | status;
	}
	
	//Return OK
	return RENDER_OK;
}

//Function to erase a string written to the screen
//Takes in the top-left logical pixel address and the length
//Calls EraseChar until the whole string is erased
//Returns status
RenderStatus PW8MonoEraseString(uint16_t address, uint8_t length){
	
	//Copy address to change later
	uint16_t pix_add = address;
	
	//Parse address into x and y
	uint8_t x = (pix_add >> 8) & 0x00FF;
	uint8_t y = pix_add & 0x00FF;
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Iterate through all chars in string
	for(uint8_t i = 0; i < length; i++){
		
		//Update address
		pix_add = ((x << 8) & 0xFF00) | y;
		
		//Attempt to clear character
		//If error, return error
		if((status = PW8MonoEraseChar(pix_add)) != RENDER_OK){
			return status;
		}
		
		//Move on to the next char address
		x += 5;
	}
	
	//Return OK
	return RENDER_OK;
}

//Function to clear the screen
//No inputs
//Just calls St7789Clear from the driver
//Returns status
RenderStatus PW8MonoClearScreen(void){
	
	//Placeholder status
	RenderStatus status = RENDER_UNKNOWN_ERROR;
	
	//Attempt to clear the screen
	//If error, return error
	if((status = St7789Clear()) != RENDER_OK){
		return RENDER_DOWNSTREAM_ERROR | status;
	}
	
	//Return OK
	return RENDER_OK;
}