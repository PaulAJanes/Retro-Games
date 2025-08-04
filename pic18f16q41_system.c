#include <xc.h>
#include "pic18f16q41_system.h"

//Function to initialize pins for input/output
//Takes no inputs
//Initializes pins for their intended tasks
//Returns OK
SystemStatus PinInit(void){
	
	//Sets B5 and B7 as outputs for ST7789
	TRISB5 = 0;
    TRISB7 = 0;
    ANSELB5 = 0;
    ANSELB7 = 0;
    
	//Sets B4 and B6 as outputs for SPI
    TRISB4 = 0;
    TRISB6 = 0;
    ANSELB4 = 0;
    ANSELB6 = 0;
	
	//Sets A2 as output for PWM
	TRISA2 = 0;
	ANSELA2 = 0;
	
	//Set C3 through C7 as inputs for buttons
	TRISC3 = 1;
	TRISC4 = 1;
	TRISC5 = 1;
	TRISC6 = 1;
	TRISC7 = 1;
	
	//Set C3 through C7 to digital
	ANSELC3 = 0;
	ANSELC4 = 0;
	ANSELC5 = 0;
	ANSELC6 = 0;
	ANSELC7 = 0;
	
	//Enable internal pull-up resistors for C3 through C7
	//This sets them to active LO
	WPUCbits.WPUC3 = 1;
	WPUCbits.WPUC4 = 1;
	WPUCbits.WPUC5 = 1;
	WPUCbits.WPUC6 = 1;
	WPUCbits.WPUC7 = 1;
    
	//Initializes pin values
    LATB4 = 0;
    LATB5 = 0;
    LATB6 = 0;
    LATB7 = 1;
	LATA2 = 1;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to initialize system clock
//No inputs
//Sets clock to high-frequency internal oscillator
//Sets speed to max, 64MHz
//Allows clock to achieve max speed
//Enables high-frequency internal oscillator
//Outputs OK
SystemStatus ClockInit(void){
	
	//Select HFINTOSC
	OSCCON1 = 0x60;
	
	//Set Freq = 64MHz
    OSCFRQ = 0x08;
	
	//Unlock max frequency
    OSCTUNE = 0x1F;
	
	//Enable HFINTOSC
    OSCEN = 0x60;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to initialize Timer2 for tick counting
//No inputs
//Sets T2 config bits based on datasheet to get ~ 1ms per count
//Sets period to 984 / FPS
//984 because the input clock is 31.5kHz with a 1:32 prescaler = ~984Hz for the timer
//Returns status
SystemStatus TickClockInit(uint8_t fps){
	
	//Validate fps <= MAX_FPS
	if(fps > MAX_FPS){
		fps = MAX_FPS;
	}
	
	//Timer off, 1:32 prescaler, 1:1 postscaler
	T2CON = 0x50;
	
	//Sync to system clock for stability
	//Software one-shot mode
	T2HLT = 0xA8;
	
	//Select 31.5kHz MFINTOSC as source
	T2CLKCON = 0x06;
	
	//Route Timer2 output to Timer2 reset, ensuring the ON bit is cleared on count finish
	T2RST = 0x01;
	
	//Set period to achieve ~MAX_FPS
	T2PR = 984 / fps;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to initialize PWM for audio
//No inputs
//Enables and selects clock
//Returns status
SystemStatus PWMInit(void){
	
	//Clears EN bit of PWM1CON, disabling PWM1
	PWM1CONbits.EN = 0;
	
	//Select MFINTOSC as clock source
	PWM1CLK = 0x06;
	
	//Enable PWM1
	PWM1CONbits.EN = 1;
	
	//Set PWM output pin to C2
	RA2PPS = 0x0A;
	
	//Allow max slew rate
	SLRCONA = 0x00;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to disable PWM
//No inputs
//Clears EN bit of PWM control register
//This will require re-initializing PWM with the PWMInit function to use it again
//Returns status
SystemStatus PWMDisable(void){
	
	//Clears EN bit of PWM1CON, disabling PWM1
	PWM1CONbits.EN = 0;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to set duty cycle for PWM
//Takes in duty cycle as 16-bit number
//Sets HI and LO bytes of duty cycle register
//Returns status
SystemStatus PWMSetDutyCycle(uint16_t cycle){
	
	//Set HI byte to MSB of input
	PWM1S1P1H = (cycle >> 8) & 0x00FF;
	
	//Set LO byte to LSB of input
	PWM1S1P1L = cycle & 0x00FF;
	
	PWM1CONbits.LD = 1;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to select prescaler for PWM
//Takes in prescaler values
//Sets value directly in correct register
//Returns status
SystemStatus PWMSetPrescaler(uint8_t scale){
	
	//Set prescale register to input scale value
	PWM1CPRE = scale;
	
	PWM1CONbits.LD = 1;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to start a tick
//No inputs
//Just turns the timer on
//Returns status
SystemStatus StartTick(void){
	
	//Enable timer
	T2CONbits.ON = 1;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to check for tick completion
//Takes in a pointer to the tick "boolean"
//Sets tick to opposite of ON bit
//Returns status
SystemStatus CheckTick(uint8_t* tick){
	
	//Check if Timer2 is running
	//I chose to do implement the logic this way to avoid strange behavior from T2CONbits.ON
	//In case it returns the whole byte or returns 0x80 or other unwanted values
	if(T2CONbits.ON == 1){
		
		//No tick if still running
		*tick = 0;
	} else {
		
		//Yes tick if not running
		*tick = 1;
	}
	
	//Return OK
	return SYSTEM_OK;
}

//Function to block CPU for x ms
//Takes in number of ms to delay
//Blocks CPU with NOP for x ms
//Returns OK
//Should be quite accurate after ClockInit
SystemStatus DelayMs(uint16_t ms){
	//Enable Timer 0
    T0CON0 |= 0x80;
    
    //Set 1:16 prescaler, FOSC/4
    //Gives 1us per tick
    T0CON1 = 0x44;
    
    //Set timer period to 249, ie 250 ticks per period
    //Gives 250us per period
    TMR0H = 0xF9;
    
    //Reset timer to 0
    TMR0L = 0;
    
    //Byte to track toggling output bit
    uint8_t match = T0CON0 & 0x20;
    
    //While loop to count up to input delay
    while(ms--){
        
        //For loop to count 4 periods
        //Gives 1000us = 1ms per loop
        for(uint8_t i = 0; i < 4; i++){
            
            //While loop to wait for period match
            while((T0CON0 & 0x20) == match){
                //NOP
            }
            
            //Toggle comparitor bit
            match ^= 0x20;
        }
    }
    
    //Disable timer
    T0CON0 &= 0x7F;
	
	//Return OK
	return SYSTEM_OK;
}

//Function to initialize SPI1
//No inputs
//Configures for max baud and slew
//Configures master mode, auto send
//Configures mode 3, transmit only
//Returns OK
SystemStatus Spi1TxInit(void){
	
	//Disable SPI1
	SPI1CON0bits.EN = 0;
    
	//Set baud rate and slew rate to max
    SPI1BAUD = 0;
    SLRCONB = 0x00;
	
	//Set RB4 to SDA and RB6 to SCK
    RB4PPS = 0x1C;
    RB6PPS = 0x1B;
    
	//Initial config
	//Set master mode
    SPI1CON0bits.MST = 1;
	
	//BMODE = 1 allows automatic data transfer when buffer is not empty
    SPI1CON0bits.BMODE = 1;
	
	//Set SPI mode 3
    SPI1CON1bits.CKE = 0;
    SPI1CON1bits.CKP = 1;
	
	//Set TX only
    SPI1CON2bits.TXR = 1;
    SPI1CON2bits.RXR = 0;
    
    DelayMs(5);
    
	//Enable SPI1
    SPI1CON0bits.EN = 1;
    
    DelayMs(10);
	
	//Return OK
	return SYSTEM_OK;
}

//Function to send data over SPI1
//Takes pointer to data and number of bytes to send
//Fills buffer then waits for buffer to empty
//Clears transmit write error
//Repeats until finished
//Returns OK
SystemStatus Spi1Send(uint8_t* data, uint8_t length){
	//Iterand
    uint8_t i = 0;
    
    //While loop to iterate through array
    while(i < length){
           
        //Another while loop to fill buffer
        //Terminates when write error detected
        while(!SPI1STATUSbits.TXWE){
            
            //Validate that we haven't exceeded data amount
            if(i >= length){
                
                //Increment i to offset later decrement
                i++;
                
                //Exit loop
                break;
            }
            
            //Shove data into buffer and increment
            SPI1TXB = data[i++];
        }
        
        //Once the buffer is full, wait for it to be empty
        while(!SPI1STATUSbits.TXBE){
             
        }
        
        //Clear write error bit to allow writing to continue
        SPI1STATUSbits.TXWE = 0;
        
        //Decrement iterand to go back to missed byte
        i--;
    }
	
	//Return OK
	return SYSTEM_OK;
}