#include "game_art.h"

//Font, letters and numbers
//Capital letters only
const uint8_t LETTER_A[3] = {0x03, 0xE4, 0xF0};

const uint8_t LETTER_B[3] = {0xFD, 0x6A, 0xA0};

const uint8_t LETTER_C[3] = {0x74, 0x62, 0xA0};

const uint8_t LETTER_D[3] = {0xFC, 0x62, 0xE0};

const uint8_t LETTER_E[3] = {0xFD, 0x6B, 0x10};

const uint8_t LETTER_F[3] = {0xFD, 0x29, 0x00};

const uint8_t LETTER_G[3] = {0x74, 0x66, 0xA0};

const uint8_t LETTER_H[3] = {0xF9, 0x09, 0xF0};

const uint8_t LETTER_I[3] = {0x04, 0x7F, 0x10};

const uint8_t LETTER_J[3] = {0x94, 0x7D, 0x00};

const uint8_t LETTER_K[3] = {0xF9, 0x15, 0x10};

const uint8_t LETTER_L[3] = {0xF8, 0x42, 0x10};

const uint8_t LETTER_M[3] = {0xFA, 0x08, 0x8F};

const uint8_t LETTER_N[3] = {0xFA, 0x09, 0xF0};

const uint8_t LETTER_O[3] = {0x74, 0x62, 0xE0};

const uint8_t LETTER_P[3] = {0xFD, 0x28, 0x80};

const uint8_t LETTER_Q[3] = {0x74, 0x64, 0xD0};

const uint8_t LETTER_R[3] = {0xFD, 0x2C, 0x90};

const uint8_t LETTER_S[3] = {0xED, 0x6B, 0x70};

const uint8_t LETTER_T[3] = {0x04, 0x3F, 0x00};

const uint8_t LETTER_U[3] = {0xF0, 0x43, 0xE0};

const uint8_t LETTER_V[3] = {0xF0, 0x45, 0xC0};

const uint8_t LETTER_W[3] = {0xF0, 0x4C, 0x1F};

const uint8_t LETTER_X[3] = {0xD9, 0x09, 0xB0};

const uint8_t LETTER_Y[3] = {0xC1, 0x06, 0x4C};

const uint8_t LETTER_Z[3] = {0x8C, 0xEB, 0x90};

const uint8_t NUMBER_0[3] = {0x03, 0xA2, 0xE0};

const uint8_t NUMBER_1[3] = {0x02, 0x7E, 0x10};

const uint8_t NUMBER_2[3] = {0x4C, 0xEA, 0x90};

const uint8_t NUMBER_3[3] = {0x8D, 0x6A, 0xA0};

const uint8_t NUMBER_4[3] = {0xE1, 0x09, 0xF0};

const uint8_t NUMBER_5[3] = {0xED, 0x6B, 0x70};

const uint8_t NUMBER_6[3] = {0xFD, 0x6B, 0x70};

const uint8_t NUMBER_7[3] = {0x8C, 0xA9, 0x80};

const uint8_t NUMBER_8[3] = {0xFD, 0x6B, 0xF0};

const uint8_t NUMBER_9[3] = {0xED, 0x6B, 0xF0};

const uint8_t CHAR_SPACE[3] = {0x00, 0x00, 0x00};

const uint8_t CHAR_COMMA[3] = {0x00, 0x4C, 0x00};

const uint8_t CHAR_PERIOD[3] = {0x00, 0xC6, 0x00};

const uint8_t CHAR_APOSTROPHE[3] = {0x01, 0x30, 0x00};

const uint8_t CHAR_QUESTION[3] = {0x44, 0x2A, 0xC0};

const uint8_t CHAR_COLON[3] = {0x00, 0x36, 0x00};

const uint8_t CHAR_EXCLAIM[3] = {0x00, 0x3A, 0x00};

const uint8_t SYMBOL_ARROW[3] = {0x21, 0x2A, 0xE2};

const uint8_t CHAR_QUOTES[3] = {0x26, 0x09, 0x80};

const uint8_t CHAR_SEMI[3] = {0x00, 0x54, 0x00};

const uint8_t CHAR_DOLLAR[3] = {0x6D, 0x7F, 0x5B};

const uint8_t CHAR_HYPHEN[3] = {0x01, 0x08, 0x40};

const uint8_t CHAR_LFPARENTH[3] = {0x00, 0x1D, 0x10};

const uint8_t CHAR_RTPARENTH[3] = {0x04, 0x5C, 0x00};
//End font

//Empty art
const uint8_t EMPTY[13] = {0};

//Error sprite artwork
const uint8_t ERROR[13] = {0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96};

const uint8_t DEATH_LINE_1[] = "You";
const uint8_t DEATH_LINE_2[] = "Died.";
const uint8_t DEATH_LINE_3[] = "";
const uint8_t DEATH_LINE_4[] = "RIP";
const uint8_t DEATH_LINE_5[] = "Dingus";

const uint8_t WIN_LINE_1[] = "You";
const uint8_t WIN_LINE_2[] = "Won!";
const uint8_t WIN_LINE_3[] = "";
const uint8_t WIN_LINE_4[] = "Congrats";
const uint8_t WIN_LINE_5[] = "Dingus";

const uint8_t* DEATH_MESSAGE[] = {DEATH_LINE_1, DEATH_LINE_2, DEATH_LINE_3, DEATH_LINE_4, DEATH_LINE_5};

const uint8_t DEATH_LENGTH = 5;

const uint8_t DEATH_LINE_LENGTHS[] = {3, 5, 0, 3, 6};

const uint8_t* WIN_MESSAGE[] = {WIN_LINE_1, WIN_LINE_2, WIN_LINE_3, WIN_LINE_4, WIN_LINE_5};

const uint8_t WIN_LENGTH = 5;

const uint8_t WIN_LINE_LENGTHS[] = {3, 4, 0, 8, 6};

//Function to fetch artwork for a given Sprite
//Takes in Sprite type and flags
//Validates input and returns correct size of correct type
const uint8_t* GetSpriteArt(uint8_t type, uint8_t flags){
	
	return ERROR;
}

//Function to return font art for char a-z A-Z and 0-9
//Takes the character as input
//Looks up and returns corresponding art
//Returns error art for invalid input
const uint8_t* GetFont(uint8_t character){
	
	//Switch statement to lookup character art
	//Case-insensitive
	//The ASCII code for a number is treated the same as the number
	switch(character){
		case 'a':
		case 'A':
			return LETTER_A;
		case 'b':
		case 'B':
			return LETTER_B;
		case 'c':
		case 'C':
			return LETTER_C;
		case 'd':
		case 'D':
			return LETTER_D;
		case 'e':
		case 'E':
			return LETTER_E;
		case 'f':
		case 'F':
			return LETTER_F;
		case 'g':
		case 'G':
			return LETTER_G;
		case 'h':
		case 'H':
			return LETTER_H;
		case 'i':
		case 'I':
			return LETTER_I;
		case 'j':
		case 'J':
			return LETTER_J;
		case 'k':
		case 'K':
			return LETTER_K;
		case 'l':
		case 'L':
			return LETTER_L;
		case 'm':
		case 'M':
			return LETTER_M;
		case 'n':
		case 'N':
			return LETTER_N;
		case 'o':
		case 'O':
			return LETTER_O;
		case 'p':
		case 'P':
			return LETTER_P;
		case 'q':
		case 'Q':
			return LETTER_Q;
		case 'r':
		case 'R':
			return LETTER_R;
		case 's':
		case 'S':
			return LETTER_S;
		case 't':
		case 'T':
			return LETTER_T;
		case 'u':
		case 'U':
			return LETTER_U;
		case 'v':
		case 'V':
			return LETTER_V;
		case 'w':
		case 'W':
			return LETTER_W;
		case 'x':
		case 'X':
			return LETTER_X;
		case 'y':
		case 'Y':
			return LETTER_Y;
		case 'z':
		case 'Z':
			return LETTER_Z;
		case 0:
		case '0':
			return NUMBER_0;
		case 1:
		case '1':
			return NUMBER_1;
		case 2:
		case '2':
			return NUMBER_2;
		case 3:
		case '3':
			return NUMBER_3;
		case 4:
		case '4':
			return NUMBER_4;
		case 5:
		case '5':
			return NUMBER_5;
		case 6:
		case '6':
			return NUMBER_6;
		case 7:
		case '7':
			return NUMBER_7;
		case 8:
		case '8':
			return NUMBER_8;
		case 9:
		case '9':
			return NUMBER_9;
		case ',':
			return CHAR_COMMA;
		case '.':
			return CHAR_PERIOD;
		case '\'':
			return CHAR_APOSTROPHE;
		case '?':
			return CHAR_QUESTION;
		case ':':
			return CHAR_COLON;
		case '!':
			return CHAR_EXCLAIM;
		case ARROW_SYMBOL:
			return SYMBOL_ARROW;
		case '"':
			return CHAR_QUOTES;
		case ';':
			return CHAR_SEMI;
		case '$':
			return CHAR_DOLLAR;
		case '-':
			return CHAR_HYPHEN;
		case '(':
			return CHAR_LFPARENTH;
		case ')':
			return CHAR_RTPARENTH;
		case ' ':
		default:
			return CHAR_SPACE;
	}
}
