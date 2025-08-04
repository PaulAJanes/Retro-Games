#include "buttons.h"

uint8_t IsPow2(uint8_t pow2);

//Function to determine if a number is a power of 2
//Takes in the number in question
//Iterates through each bit, checking for equality with known powers of 2
//Returns the power of 2 if it is a power of 2
//Returns 0 if not a power of 2
uint8_t IsPow2(uint8_t pow2){
	
	//Iterate through the 8 bits of the byte
	for(uint8_t i = 0; i < 8; i++){
		
		//Check if pow2 equals a known power of 2
		if(pow2 == (0x01 << i)){
			
			//Return input if so
			return pow2;
		}
	}
	
	//Return 0
	return 0;
}

//Function to poll all 5 buttons of this setup
//Takes in a pointer to the button status flags
//Polls PORTC, where the buttons are, and stores the result in the input location
//Returns status
ButtonStatus PollAll(uint8_t* buttons){
	
	//Poll PORTC and store
	*buttons = (PORTC & 0xF8);
	
	//Return OK
	return BUTTONS_OK;
}

//Function to poll a single button
//Takes in a pointer to a byte that holds only one bit
//That bit corresponds to the button being polled
//Simply masks PORTC with the input button
//Returns status
ButtonStatus PollOne(uint8_t* button){
	
	//Validates the input is exactly one button in bits [7..3]
	if((IsPow2(*button) == 0) || (*button < 8)){
		
		//Return invalid input if invalid
		return BUTTONS_INVALID_INPUT;
	}
	
	//Masks PORTC with input valid button
	*button &= PORTC;
	
	//Return ok
	return BUTTONS_OK;
}