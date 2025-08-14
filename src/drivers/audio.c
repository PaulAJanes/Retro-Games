#include "audio.h"

//Lookup table to hold the 32 predefined tones
//First 24 are the predefined notes F#3 through F5
//Final 8 are beeps and buzzes and such
//Notes are defined as period values to a PWM running on a 31.5kHz clock
//Max error is about 15 cents on E5, followed by 11 cents on C#5
//All other notes are within 10 cents of "true"
//0x00: silence, rest
//0xFF: deep buzz
//0xA4, 0x88, 0x7A, 0x65, 0x48, 0x23: beeps and buzzes of various frequencies
const uint8_t NOTES[32] = {
							0xA8, 0x9E, 0x95, 0x8D, 0x85, 0x7E, 0x76, 0x70,
							0x69, 0x63, 0x5E, 0x58, 0x53, 0x4F, 0x4A, 0x46,
							0x42, 0x3E, 0x3B, 0x37, 0x34, 0x31, 0x2E, 0x2C,
							0x00, 0xFF, 0xA4, 0x88, 0x7A, 0x65, 0x48, 0x23
							};

//Lookup table to hold the 32 notes used per game instance
//NOTES is accessible to the outside, so games can choose the default notes
uint8_t note_bank[32];

//Function to initialize audio
//Takes in notes array
//Sets note_bank to note array values
//Initializes PWM
//Returns status
AudioStatus AudioInit(uint8_t* notes){
    
    //Copy input to note_bank
    for(uint8_t i = 0; i < 32; i++){
        note_bank[i] = notes[i];
    }
    
    //Placeholder status
    AudioStatus status = AUDIO_UNKNOWN_ERROR;
    
    //Attempt to init PWM, if error return error
    if((status = PWMInit()) != AUDIO_OK){
        return status;
    }
    
    //Return OK
    return AUDIO_OK;
}

//Function to play a tone
//Takes in the tone as 8 bits
//[7..5] are the tone length, defined on a per-game basis
//Recommended length definition is 2^n ticks, so 000 is 1 and 111 is 128, but you do you
//[4..0] are the tone itself, see NOTES
//Parses the input tone
//Calls PWMSetDutyCycle and PWMSetPrescaler
//Returns status
AudioStatus PlayTone(uint8_t tone){
	
	//Parses tone from input
	uint8_t note = tone & 0x1F;
    
    //Calculates duty cycle for each tone, based on frequency
    uint16_t cycle = (uint16_t) ((note_bank[note] >> 1) - (note_bank[note] < 64) - (note_bank[note] < 96) - (note_bank[note] < 128));
	
	//Placeholder status
	AudioStatus status = AUDIO_UNKNOWN_ERROR;
	
	//Try to set duty cycle
	//Return error if error
	if((status = PWMSetDutyCycle(cycle)) != AUDIO_OK){
		return status;
	}
	
	//Try to set prescaler
	//Return error if error
	if((status = PWMSetPeriod((uint16_t) note_bank[note])) != AUDIO_OK){
		return status;
	}
	
	//Return OK
	return AUDIO_OK;
}

//Function to play silent tone
//No inputs
//Sets period and duty cycle to 0x0000
//Returns status
AudioStatus Silence(){
    
    //Placeholder status
	AudioStatus status = AUDIO_UNKNOWN_ERROR;
	
	//Try to set duty cycle
	//Return error if error
	if((status = PWMSetDutyCycle(0x0000)) != AUDIO_OK){
		return status;
	}
	
	//Try to set prescaler
	//Return error if error
	if((status = PWMSetPeriod(0x0000)) != AUDIO_OK){
		return status;
	}
	
	//Return OK
	return AUDIO_OK;
}
