#ifndef MENUFUCNTIONS
#define MENUFUCNTIONS
#include <Arduino.h>
#include <Encoders.h>
#include <My_LCD.h>
#include <GeneralFunctions.h>
#include <TextFunctions.h>

#define X     0 // this is a dummy menu that isn't actually used
#define HOME  1
#define ADJ   2
#define IMGS  3
#define TEXT  4
#define FUNCTIONS 5
#define CLK   6

#define WANDS 7
#define BANDS 8
#define EDITWAND   9
#define WANDCOLOR 10
#define WANDRGB   11
#define WANDHSV   12
#define SAVEWAND  13

#define EDITBAND  14
#define BANDCOLOR 15
#define BANDRGB   16
#define BANDHSV   17
#define SAVEBAND  18

#define TEXTFONT  19
#define EDITTEXT  20

#define TEXTCOLOR 21
#define TEXTBACKGROUND 22
#define TEXTWIDTH 23
#define TEXTRUN  24
#define SAVETEXT 25
/////////////////////////////////////////////////////////////////
void throwError(){
  lcd.clear();
  lcd.write(" Error Menu Hit ");
}
void doNothing(){}
void enc2True(){
  enc2_changed = true;
}
//Wand menu functions
void pickWand(){
  bwand_num = enc2%(num_wands+1)-1; //also allow -1 for nu value
  printNums(bwand_num);
  if(enc2_changed || millis()-time_ref>50){
    time_ref = millis();
    displaySDWand(); enc2_changed=false;
  }
}
void readyPickWand(){
  num_wands = getNumBWands(false);
  setEncTo0(enc2, num_wands);
  enc2_changed = true;
}
void editWandShape(){
  start = enc2%NUM_LEDS;
  len = enc3%NUM_LEDS;
  setEncByClick(swc2, NUM_LEDS, enc2, sw2);
  setEncByClick(swc3, NUM_LEDS, enc3, sw3);
  printNums(start,len);
  displayLocalWand(start, len, 100,100,100,enc2_changed,enc3_changed);
}
void readyEditWandShape(){
  if(bwand_num==-1){
    bwand_num = getNumBWands(false); // set to new wand
    setEncTo0(enc2, start);
    setEncTo0(enc3, len);
  }
  else{
    setEncToVal(enc2, NUM_LEDS, start);
    setEncToVal(enc3, NUM_LEDS, len);
  }
}
void editWandRGB(){
  setSW2ValuesRGB();
  printNums(R,G,B);
  displayLocalWand(start, len, R,G,B, enc2_changed);
}
void editWandHSV(){
  setSW2ValuesHSV();
  mapHSVtoRGB();
  printNums(H,S,V);
  displayLocalWand(start, len, R,G,B, enc2_changed);
}
void saveWand(){
  printNums(bwand_num);
  delay(1500);
  writeWandFile();
  num_wands = getNumBWands(false);
  menu_index = WANDS;
  blacklist = true;
}

// Band menu functions
void pickBand(){
  bwand_num = enc2%(num_band_files+1)-1; //also allow -1 for nu value
  if(swc3%2==0){
    band_blend = 0;
  }
  else{
    band_blend = enc3%100;
  }
  printNums(bwand_num, band_blend);
  if(enc2_changed || enc3_changed || millis()-time_ref>50){
    time_ref = millis();
    displaySDBand(); enc2_changed=false; enc3_changed = false;}
}
void readyPickBand(){
  num_band_files = getNumBWands(true);
  setEncTo0(enc2, num_band_files);
  setEncTo0(enc3,100); // set band blend % to 0
  enc2_changed = true;
}
void editBandShape(){
  num_bands  = (enc2%MAX_BAND_COLORS)+1;
  num_iters  = (enc3%MAX_BAND_ITER)+1;
  printNums(num_bands, num_iters);
  displayLocalBand(false);
}
void readyEditBandShape(){
  if(bwand_num==-1){
    bwand_num = getNumBWands(true); // set to new band
    setEncTo0(enc2, start);
    setEncTo0(enc3, len);
  }
  else{
    band_blend = 0;
    setEncToVal(enc2, NUM_LEDS, 3); // 3 colors default
    setEncToVal(enc3, NUM_LEDS, 1); // 1   iter default
  }
}
void readyEditBandColors(){
  setEncTo0(enc3, num_bands);
}
void editBandRGB(){
  bandcol = enc3%num_bands;
  setSW2ValuesRGB();
  printNums(R,G,B, bandcol + 1); // start at 1 cause counting
  displayLocalBandEdited();
  if(sw1 && move==0){
    setBandColInArrayRGB();
    ignoreSW1 = true;
  }
}
void editBandHSV(){
  bandcol = enc3%num_bands;
  setSW2ValuesHSV();
  mapHSVtoRGB();
  printNums(H,S,V, bandcol + 1);
  displayLocalBandEdited();
  if(sw1 && move==0){
    setBandColInArrayRGB();
    ignoreSW1 = true;
  }
}
void saveBand(){
  printNums(bwand_num);
  delay(1500);
  writeBandFile();
  num_band_files = getNumBWands(true);
  menu_index = BANDS;
  blacklist = true;
}

// Text menu functions
void pickString(){
  if(enc2_changed){
    enc2_changed = false;
    string_index = enc2%(num_strings+1)-1;
    readSDString(); // gets string_len, and string
    setEncTo0(enc3, string_len);
    printString(0,false);
    printNumsSkipFirst(string_index);
  }
  if(enc3_changed){
    enc3_changed = false;
    printString(enc3%(string_len-LCD_WIDTH+1),false);
  }
}




#endif
