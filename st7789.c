#include <xc.h>
#include "st7789.h"

const uint8_t soft_reset = 0x01;
const uint8_t sleep_out = 0x11;
const uint8_t access_control = 0x36;
const uint8_t format_select = 0x3A;
const uint8_t inverted = 0x21;
const uint8_t display_on = 0x29;
const uint8_t col_address = 0x2A;
const uint8_t row_address = 0x2B;
const uint8_t memory_write = 0x2C;

const uint8_t access = 0x40;
const uint8_t format = 0x55;

const uint8_t span[4] = {0x00, 0x00, 0x00, 0xEF};

//Function to initialize ST7789
//Takes no input
//Sends various init commands and data
//Returns OK
SystemStatus St7789Init(void){
	
	//Pull reset low, triggering hardware reset
	LATB7 = 0;
    DelayMs(10);
    
	//Release reset, allowing interaction
    LATB7 = 1;
    DelayMs(120);
    
	//DC to command, software reset
    LATB5 = 0;
    DelayMs(5);
    SPI1TXB = soft_reset;
    
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(120);
    
	//End sleep mode
    SPI1TXB = sleep_out;
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(120);
    
    //Sets refresh and addressing directions
    SPI1TXB = access_control;
    
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(5);
    
	//DC to data
	//Sets to top to bottom, left to right
    LATB5 = 1;
    DelayMs(5);
    SPI1TXB = access;
    
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(5);
    
	//Quick delay to allow buffer to empty
    while(!SPI1STATUSbits.TXBE){
        
    }
    
    DelayMs(5);
    
	//DC to command, RGB format select
    LATB5 = 0;
    SPI1TXB = format_select;
    
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(5);
    
	//DC to data, select 16-bit format
    LATB5 = 1;
    DelayMs(5);
    SPI1TXB = format;
    
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(5);
    
	//DC to command, turn on inversion
	//This actually turns off inversion, go figure
    LATB5 = 0;
    DelayMs(5);
    SPI1TXB = inverted;
    
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(5);
    
	//Turn on display
    SPI1TXB = display_on;
    
    while(SPI1CON2bits.BUSY){
        
    }
    
    DelayMs(5);
    
    //Allow buffer to clear
     while(!SPI1STATUSbits.TXBE){
        
    }
    
	//Clear screen and return status upstream
    return St7789Clear();
}

//Function to send a command to the ST7789
//Takes a pointer to the command as input
//Sends the command via Spi1Send
//Returns SystemStatus
SystemStatus St7789Cmd(const uint8_t* cmd){
	
	//Set DC to command
	LATB5 = 0;
    
	//Send and return status upstream
    return Spi1Send(cmd, 1);
}

//Function to send data to the ST7789
//Takes a pointer to the data and length as input
//Sends the data via Spi1Send
//Returns SystemStatus
SystemStatus St7789Data(uint8_t* data, uint16_t length){
	
	//Set DC to data
	LATB5 = 1;
    
	//Send and return status upstream
    return Spi1Send(data, length);
}

//Function to set every pixel on the ST7789 to one color
//Takes hi and lo bytes of the color
//Addresses the whole screen
//Sends color 57600 times
//Returns SystemStatus
SystemStatus St7789Flash(uint8_t color_hi, uint8_t color_lo){
	
	uint8_t color[2];
	
	color[0] = color_hi;
	color[1] = color_lo;
	
	//Status placeholder
	SystemStatus status = SYSTEM_OK;
	
	//Set column address space to whole screen
	St7789Cmd(&col_address);
    St7789Data(span, 4);
    
	//Set row address space to whole screen
    St7789Cmd(&row_address);
    St7789Data(span, 4);
    
	//Write command
    St7789Cmd(&memory_write);
    
	//For loop to iterate through every pixel
    for(uint8_t i = 0; i < 240; i++){
        for(uint8_t j = 0; j < 240; j++){
			
			//Send data, early return if error
            if((status = St7789Data(color, 2)) != SYSTEM_OK){
				return status;
			}
        }
    }
	
	//Return status
	return status;
}

//Function to set every pixel on the ST7789 to black
//Takes no input
//Calls St7789Flash with black
//Returns SystemStatus
SystemStatus St7789Clear(void){
	
	return St7789Flash(0x00, 0x00);
}

//Function to draw an image to the screen
//Takes in start and end row and column
//Takes in pointer to 16-bit RGB artwork and length
//Formats data and sends to ST7789 using St7789Data
//Passes SystemStatus upstream
SystemStatus St7789Draw(uint8_t start_row, uint8_t end_row, uint8_t start_col, uint8_t end_col, uint8_t* art, uint8_t length){
	
	//Build 4-Byte row and column data
	uint8_t rows[4] = {0};
	uint8_t cols[4] = {0};
	
	rows[1] = start_row;
	rows[3] = end_row;
	
	cols[1] = start_col;
	cols[3] = end_col;
	
	//Transmit row and column data
	St7789Cmd(&col_address);
	St7789Data(cols, 4);
	
	St7789Cmd(&row_address);
	St7789Data(rows, 4);
	
	//Begin write
	St7789Cmd(&memory_write);
	
	//Write and return status
	return St7789Data(art, length);
}