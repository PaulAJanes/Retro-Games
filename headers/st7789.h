#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>
#include "pic18f16q41_system.h"

extern const uint8_t soft_reset;
extern const uint8_t sleep_out;
extern const uint8_t access_control;
extern const uint8_t format_select;
extern const uint8_t inverted;
extern const uint8_t display_on;
extern const uint8_t col_address;
extern const uint8_t row_address;
extern const uint8_t memory_write;

extern const uint8_t access;
extern const uint8_t format;

extern const uint8_t span[4];

SystemStatus St7789Init(void);
SystemStatus St7789Cmd(const uint8_t* cmd);
SystemStatus St7789Data(uint8_t* data, uint16_t length);
SystemStatus St7789Flash(uint8_t color_hi, uint8_t color_lo);
SystemStatus St7789Clear(void);
SystemStatus St7789Draw(uint8_t start_row, uint8_t end_row, uint8_t start_col, uint8_t end_col, uint8_t* art, uint8_t length);

#endif
