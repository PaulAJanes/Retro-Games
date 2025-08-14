#ifndef GAME_ART_H
#define GAME_ART_H

#include <stdint.h>

#define ARROW_SYMBOL 10

extern const uint8_t* DEATH_MESSAGE[];
extern const uint8_t DEATH_LENGTH;
extern const uint8_t DEATH_LINE_LENGTHS[];

extern const uint8_t* WIN_MESSAGE[];
extern const uint8_t WIN_LENGTH;
extern const uint8_t WIN_LINE_LENGTHS[];

const uint8_t* GetSpriteArt(uint8_t type, uint8_t flags);
const uint8_t* GetFont(uint8_t character);

#endif
