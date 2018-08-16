#ifndef DEFINITIONS
#define DEFINITIONS

#define BUFF_SIZE       24 // for reading data from files
#define STRING_SIZE     60 // for reading long strings from files
#define LCD_WIDTH       16 //
#define FONT_NAME_LEN   11 // space for fontname on LCD
#define ARRAY_LENGTH    16 // used for num_locations and curs_locations
#define MAX_BAND_COLORS 16 //
#define MAX_BAND_ITER   16
#define MYSCALE         256 // so num%scale returns 0-255, each LED has a char

// for TextFunctions.h
#define NUM_TYPES   4 // number of different character types
#define NUM_SYMS    33 // number of symbols
#define NUM_ALPHA   26 //length of alphabet, letters only
#define NUM_DIGIT   10 // digits 0-9
#define INPERPIX    0.2734035
#define MAXDISPTIME 420  // the max number of seconds to dsplay text (7mins)
// for FUNCTIONS
#define QQ          -300 // a special number which is read as blank in the bwand_q
#define MAXBLINK    2000 // max of 2 seconds on blink
#define DATA_PIN    A6
#define NUM_LEDS    288  // length of LED string


#endif
