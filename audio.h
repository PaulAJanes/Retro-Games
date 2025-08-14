#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include "pic18f16q41_system.h"

#define AUDIO_OK 0
#define AUDIO_INVALID_INPUT 1
#define AUDIO_DOWNSTREAM_ERROR 32
#define AUDIO_FAILED_OP 64
#define AUDIO_UNKNOWN_ERROR 128

extern const uint8_t NOTES[32];
extern uint8_t note_bank[32];

typedef uint8_t AudioStatus;

AudioStatus AudioInit(uint8_t* notes);
AudioStatus PlayTone(uint8_t tone);
AudioStatus Silence(void);

#endif