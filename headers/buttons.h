#ifndef BUTTONS_H
#define BUTTONS_H

#include <xc.h>
#include <stdint.h>
#include "pic18f16q41_system.h"

#define BUTTONS_OK 0
#define BUTTONS_INVALID_INPUT 1
#define BUTTONS_DOWNSTREAM_ERROR 32
#define BUTTONS_FAILED_OP 64
#define BUTTONS_UNKNOWN_ERROR 128

typedef uint8_t ButtonStatus;

ButtonStatus PollAll(uint8_t* buttons);
ButtonStatus PollOne(uint8_t* button);

#endif
