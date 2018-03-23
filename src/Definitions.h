#ifndef DEFINITIONS
#define DEFINITIONS

#define BUFF_SIZE       16 // for reading strings from files
#define STRING_SIZE     60 // for reading long strings from files
#define LCD_WIDTH       16 //
#define FONT_NAME_LEN   11 // space for fontname on LCD
#define ARRAY_LENGTH    16 // used for num_locations and curs_locations
#define NUM_LEDS       288 // length of LED string
#define MAX_BAND_COLORS 16 //
#define MAX_BAND_ITER   16
#define MYSCALE        256 // so num%scale returns 0-255, each LED has a char

// for TextFunctions.h
#define NUM_TYPES  4 // number of different character types
#define NUM_SYMS  33 // number of symbols
#define NUM_ALPHA 26 //length of alphabet, letters only
#define NUM_DIGIT 10 // digits 0-9

#define DATA_PIN    A6
#define NUM_LEDS    288
#endif
