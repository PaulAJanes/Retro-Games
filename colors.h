#ifndef COLORS_H
#define COLORS_H

/*
	Header file to facilitate 1- and 8-bit color conversions
	Does not handle other conversions, such as 12-bit to 24-bit
	Assumes any 2- or 4-bit palettes are handled by local lookup tables
	All conversions duplicate bits or pad 0s to mimic resolution
	Author: Paul Janes
	Date: 7/14/2025
*/

#include <stdint.h>

//Lookup tables for monochrome conversions
static const uint8_t bits_1to8[2] = {0x00, 0xFF};
static const uint16_t bits_1to16[2] = {0x0000, 0xFFFF};
static const uint32_t bits_1to32[2] = {0x00000000, 0xFFFFFFFF};

//Allow calls to non-existent tables
#define bits_1to12 bits_1to16
#define bits_1to18 bits_1to32
#define bits_1to24 bits_1to32

//Convert 332 RGB to 444 RGB
#define bits_8to12(c) \
	((((c) & 0xE0) << 4) | (((c) & 0x1C) << 3) | \
	(((c) & 0x03) << 2) | ((c) & 0x03))
	
//Convert 332 RGB to 565 RGB
#define bits_8to16(c) \
	((((c) & 0xE0) << 8) | (((c) & 0xC0) << 5) | \
	(((c) & 0x1C) << 6) | (((c) & 0x1C) << 3) | \
	(((c) & 0x03) << 3) | (((c) & 0x03) << 1))
	
//Convert 332 RGB to 666 RGB
#define bits_8to18(c) \
	((((c) & 0xE0) << 24) | (((c) & 0xFC) << 21) | \
	(((c) & 0x1F) << 18) | \
	(((c) & 0x03) << 16) | (((c) & 0x03) << 14))
	
//Convert 332 RGB to 888 RGB
#define bits_8to24(c) \
	((((c) & 0xE0) << 24) | (((c) & 0xE0) << 21) | (((c) & 0xC0) << 18) | \
	(((c) & 0x1C) << 19) | (((c) & 0x1C) << 16) | (((c) & 0x18) << 13) | \
	(((c) & 0x03) << 14) | (((c) & 0x03) << 12) | (((c) & 0x03) << 10) | (((c) & 0x03) << 8))
	
#endif