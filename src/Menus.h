#ifndef MENUS
#define MENUS
#include <Definitions.h>
#include <MenuFunctions.h>
#include <avr/pgmspace.h>

#define MAX_SUBMENUS  7
#define MAX_FUNCTIONS 3
struct m{
  short connections[MAX_SUBMENUS];
  //short num_menus; this is stored in the menu file itself
  void (*functions[MAX_FUNCTIONS])();
};

//  first array is {parent, submenu1, submenu2, submenu3, submenu4, submenu5, submenu6 }
//  second array is {whileHereFunction, transitionHereFunction, transitionBackFunction }

const m menu_table[26] PROGMEM = {
  //#region Main menu
    /*  X_0  */
    {{HOME,       HOME,       HOME,       HOME,       HOME,       HOME,       HOME},
    {throwError,throwError,throwError}},
    /*  HOME_1  */
    {{HOME,       ADJ,        /*IMGS*/HOME,       TEXT,       /*FUNCTIONS*/HOME,  /*CLK*/HOME,        X},
    {doNothing,stickOff,doNothing}},
    /*  ADJ_2  */
    {{HOME,       WANDS,      BANDS,      HOME,       X,          X,          X},
    {doNothing,doNothing,doNothing}},
    /*  IMGS_3  */
    {{HOME,       X,          X,          X,          X,          X,          X},
    {doNothing,doNothing,doNothing}},
    /*  TEXT_4  */
    {{HOME,       TEXTFONT,   EDITTEXT,   HOME,       X,          X,          X},
    {pickString,readyPickString,doNothing}}, // NOT DONE!
    /*  FUNCTIONS_5  */
    {{HOME,       X,          X,          X,          X,          X,          X},
    {doNothing,doNothing,doNothing}},
    /*  CLK_6  */
    {{HOME,       X,          X,          X,          X,          X,          X},
    {doNothing,doNothing,doNothing}},
  //#endregion

  /*  WANDS_7  */
  {{ADJ,        EDITWAND,    ADJ,       X,          X,          X,          X},
  {pickWand,    readyPickWand,    stickOff}},
  /*  BANDS_8  */
  {{ADJ,        EDITBAND,    ADJ,       HOME,       X,          X,          X},
  {pickBand,    readyPickBand,   stickOff}},

  //#region // WANDS AND BANDS /////////////////////////////////////////////////

    /*  EDITWAND_9  */
    {{WANDS,      WANDCOLOR,   WANDS,      X,          X,          X,          X},
    {editWandShape,   readyEditWandShape,doNothing}},
    /*  WANDCOLOR_10  */
    {{EDITWAND,   WANDRGB,     WANDHSV,    EDITWAND,   X,          X,          X},
    {doNothing,mapRGBtoHSV,doNothing}},
    /*  WANDRGB_11  */
    {{WANDCOLOR,  SAVEWAND,   WANDCOLOR,   HOME,       X,          X,          X},
    {editWandRGB, enc2True,    mapRGBtoHSV}},
    /*  WANDHSV_12  */
    {{WANDCOLOR,  SAVEWAND,   WANDCOLOR,  HOME,        X,          X,          X},
    {editWandHSV, enc2True,    mapHSVtoRGB}},
    /*  SAVEWAND_13  */
    {{X,          HOME,          X,          X,           X,          X,          X},
    {saveWand,doNothing,doNothing}},
    /*  EDITBAND_14  */    // this is editing the iter and num bands
    {{BANDS,        WANDCOLOR,    BANDS,    HOME,       X,          X,          X},
    {editBandShape,readyEditBandShape,doNothing}},
    /*  BANDCOLOR_15  */    // this is picking RGB or HSV
    {{EDITBAND,     BANDRGB,      BANDHSV,  X,          X,          X,          X},
    {readyEditBandColors,doNothing,doNothing}},
    /*  BANDRGB_16  */
    {{BANDCOLOR,    SAVEBAND,    X,         X,          X,          X,          X},
    {editBandRGB,     enc2True,         mapRGBtoHSV}},
    /*  BANDHSV_17  */
    {{BANDCOLOR,    SAVEBAND,    X,         X,          X,          X,          X},
    {editBandHSV,   enc2True,         mapHSVtoRGB}},
    /*  SAVEBAND_18  */
    {{X,            HOME,           X,         X,          X,          X,          X},
    {saveBand,doNothing,doNothing}},
  //#endregion

  //#region Text
  /*  TEXTFONT_19  */
  {{TEXT,         TEXTCOLOR,    TEXT,          X,          X,          X,   X},
  {pickFontStyle,doNothing,doNothing}},
  /*  EDITTEXT_20  */
  {{TEXT,         SAVETEXT,     TEXT,             X,          X,          X,   X},
  {editText,readyEditText,doNothing}},

  /*  TEXTCOLOR_21  */
  {{TEXTFONT,     TEXTBACKGROUND, TEXTFONT,    HOME,          X,          X,   X},
  {setTextColors,readySetTextColors,doNothing}},
  /*  TEXTBACKGROUND_22  */
  {{TEXTCOLOR,    SETTIME,      TEXTCOLOR,   HOME,       X,          X,   X},
  {setTextSkeleton,readySetTextSkeleton,doNothing}},
  /*  SETTIME_23 */
  {{TEXTBACKGROUND,TEXTRUN,       TEXTBACKGROUND,HOME,     X,          X,   X},
  {setTextWidthAndTime,setWidth,doNothing}},
  /*  TEXTRUN_24  */
  {{SETTIME,            X,              X,          X,           X,          X,   X},
  {runText,readyRunText,doNothing}},

  /*  SAVETEXT_25  */
  {{X,            HOME,              X,          X,          X,          X,   X},
  {doNothing,writeStringFile,doNothing}}
  //#endregion

};

/*menu void PROGMEM_readMenu (const menu * sce, menu& dest)
  {
  memcpy_P (&dest, sce, sizeof (menu));
  }
  memcpy_P(menu&, const menu *, sizeof(menu));*/

#endif
