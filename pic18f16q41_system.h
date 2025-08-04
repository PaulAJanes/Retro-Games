#ifndef PIC18F16Q41_SYSTEM_H
#define PIC18F16Q41_SYSTEM_H

#include <xc.h>
#include <stdint.h>

#define OSC_CONFIG_1 0x60
#define OSC_FREQ 0x08
#define OSC_TUNE 0x1F
#define OSC_EN 0x60

#define MAX_FPS 60

#define SYSTEM_OK 0
#define SYSTEM_INVALID_INPUT 1
#define SYSTEM_DOWNSTREAM_ERROR 32
#define SYSTEM_FAILED_OP 64
#define SYSTEM_UNKNOWN_ERROR 128

typedef uint8_t SystemStatus;

SystemStatus PinInit(void);
SystemStatus ClockInit(void);
SystemStatus TickClockInit(uint8_t fps);
SystemStatus PWMInit(void);
SystemStatus PWMDisable(void);
SystemStatus PWMSetDutyCycle(uint16_t cycle);
SystemStatus PWMSetPrescaler(uint8_t scale);
SystemStatus StartTick(void);
SystemStatus CheckTick(uint8_t* tick);
SystemStatus DelayMs(uint16_t ms);
SystemStatus Spi1TxInit(void);
SystemStatus Spi1Send(uint8_t* data, uint8_t length);

#endif