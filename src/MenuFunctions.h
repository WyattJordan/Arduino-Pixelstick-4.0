#ifndef MENUFUCNTIONS
#define MENUFUCNTIONS
#include <Arduino.h>
#include <Encoders.h>
#include <My_LCD.h>
#include <GeneralFunctions.h>
#include <TextFunctions.h>
//#region Menu index definitions
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
#define TEXTBACK  22
#define SETTIME   23
#define TEXTRUN   24
#define SAVETEXT  25

#define BLINK   26
#define BLINK2  27
#define FADE    28
#define RAIN    29
#define BATT    30
#define ROOM    31
#define MORE    32
#define DOTS    33
#define LINES   34

//#endregion

void throwError(){
  lcd.clear();
  lcd.write(" Error Menu Hit ");
}
void doNothing(){}
void enc2True(){
  enc2_changed = true;
}

//#region Wand menu functions
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
//#endregion

//#region Band menu functions
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
//#endregion

//#region Text menu functions
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
void readyPickString(){
  enc2_changed = true;
  num_strings = getNumSDStrings();
  setEncTo0(enc2, num_strings);
}

void pickFontStyle(){
  font_index = enc2%num_fonts;
  bold    = swc2%2==1;
  italics = swc3%2==1;
  height_index = enc3%4;
  if(millis()-time_ref>50){
    printFontName();
    time_ref = millis();
  }
}
void editText(){
  type = swc2%NUM_TYPES;
  Serial.print("type = "); Serial.println(type);
  if(type == 1)   {charval = enc2%NUM_ALPHA+97;}   // lowercase
  else if(type==2){charval = enc2%NUM_DIGIT+48;}   // digits
  else if(type==3){charval = syms[enc2%NUM_SYMS];} // symbols
  else            {charval = enc2%NUM_ALPHA+65;}   // capitals

  letter_index = enc3%(string_len+1);
  if(letter_index > LCD_WIDTH) {start = 0;}
  else                         {start = LCD_WIDTH - letter_index + 1;}

  printStringEditing();

  if(sw3){
    sw3=false;
    if(letter_index == string_len && letter_index<STRING_SIZE-1){
      string_len++;
      enc3++; // move cursor to new space so user knows it changed
    }
    string[letter_index] = charval;
  }
}
void saveString(){
  delay(1500);
  menu_index = 1;
  blacklist = true;
  sw1 = true;
}
void readyEditText(){
  setEncTo0(enc2, NUM_TYPES);
  lcd.noBlink();
  lcd.cursor();
}
void leaveEditText(){
  lcd.noCursor();
}
void setTextColors(){
  bwand_num  = enc2%(num_wands+1)-1; // previously this was: = enc2%(num_wands+1)-1
  bwand_num2 = enc3%(num_wands+1)-1;
  if(sw2){ // swap background and text colors
    setEncToVal(enc3, num_wands+1, bwand_num);
    enc3++;
    setEncToVal(enc2, num_wands+1, bwand_num2);
    enc2++;
    sw2 = false;
  }
  //if(millis()-time_ref>50){
    readSDWand(bwand_num2);
    background.R = R; background.G = G; background.B = B;
    readSDWand(bwand_num);
    printNumsTwoNu(bwand_num, bwand_num2);
    displayTwoColors();
  //}
}
void readySetTextColors(){
  setEncToVal(enc2, num_wands, bwand_num);
  setEncToVal(enc3, num_wands, bwand_num2);
  num_wands = getNumBWands(false);
}
void setTextSkeleton(){

  start = enc2%(NUM_LEDS-pixel_height+1);
  setEncByClick(swc2, NUM_LEDS-pixel_height+1, enc2, sw2);

  short range=1;
  if(start<NUM_LEDS-pixel_height-start){range = start;}
  else{range = NUM_LEDS-pixel_height-start;}
  if(enc2_changed){
    enc2_changed = false;
    setEncToVal(enc3, range, back_width);
  }
  if(range==0){back_width = 0;}
  else{back_width = enc3%(range);}
  if(swc3%2==0){
    swc3++;
    setEncTo0(enc3, NUM_LEDS-pixel_height-start);
  }
  printNums(back_width, start);
  displayFontSkeleton();
}
void readySetTextSkeleton(){
  getTextDimensions(); // get height and width
  setEncTo0(enc2, NUM_LEDS-pixel_height+1);
}

void setTextWidthAndTime(){
  temp_width = enc2%(string_in_width*2); // can double image width
  if(enc2_changed) setEncToVal(enc3, MAXDISPTIME, temp_width/36); //when dist changes adjust time
  setEncByClick(swc2, string_in_width*2, enc2, sw2);
  //display_time = enc3%MAXDISPTIME; // max of 7 mins (420 seconds)
  setEncByClick(swc3, MAXDISPTIME, enc3, sw3);
  // just put inches here instead of % ft
  printNums(temp_width/12,(temp_width%12)/* *10/12 */, display_time);
  //Serial.print("temp_width = "); Serial.println(temp_width);
  //Serial.print("displaytime = "); Serial.println(display_time);
}
void readySetWidthTime(){
  setEncToVal(enc2, string_in_width*2, string_in_width);
  display_time = string_in_width/36.0; // assumes walking speed of 3ft/s or 36in/s
  setEncToVal(enc3, MAXDISPTIME, display_time);
  Serial.print("width in pixels = "); Serial.println(string_pixel_width);
  Serial.print("width in inches = "); Serial.println(string_in_width);
  Serial.print("time in seconds = "); Serial.println(display_time);
}
void readyRunText(){
  ignoreSW1 = true;
  stop = false;
  time_ref = millis(); // frame delay in ms
  framedelay = ((double) display_time * 1000.0)/((double) string_pixel_width);
  Serial.print("framedelay = "); Serial.println(framedelay);
}
void runText(){
    //TODO
    //  check if its an upper case, lower case, or symbol
    //  open the file
    //  read 3 bytes and write one LED for wole strip
    //  write strip and show, set delay between vertical lines
    if(!stop){
      displayFontSkeleton();
      char data[] = {'\0','\0','\0'}; // for reading RGB from file
      short dummybits = 8 - pixel_height%8;
      Serial.print("pixel height ="); Serial.println(pixel_height);
      Serial.print("dummies =");Serial.println(dummybits);
      for( int l=0; l<string_len; l++){// for each letter in the string
        char c = string[l];
        String filename = getFontDir() + getLetterFileName(c) + ".txt";
        Serial.print("letter file is: "); Serial.println(filename);
        File readFile = SD.open(filename, FILE_READ);
        readFile.read(&data, 3); // read 3 bytes into data
        //Serial.print("Byte1 ="); Serial.println(data[0]);
        //Serial.print("Byte2 ="); Serial.println(data[1]);
        //Serial.print("Byte3 ="); Serial.println(data[2]);
        int total_col_num=0;
        int len = getLetterLength(c);
        Serial.print("letter length="); Serial.println(len);
        int countreads = 0;
        bool startcount = false;
        short bit = dummybits/2; // TODO
        for(int col = 0; col<len; col++){// for each column in a letter


          time_ref2 = millis();
          total_col_num++;
          int oncount;
          for(int d=start; d<pixel_height+start; d++){ // write a column
            ignoreSW1 = true;
            if(c==' '){
              leds[d].setRGB(background.R,background.G,background.B); // tilda is a space
            }
            else{
              // bit/8 ranges 0,1,2 for which byte to use
              short byte = bit/8;
              if( ( data[byte] >> (7 - (bit - 8*byte) ) )  & 1){// if its a 1 in the data
                leds[d].setRGB(R,G,B);
                //Serial.print("=");
                if(startcount) oncount++;
              }
              else{
                leds[d].setRGB(background.R,background.G,background.B); // off
                //Serial.print(" ");
                if(!startcount) startcount = true;
                else if(oncount==6 && d>6){
                  for(int j = d-1; j>d-1-6; j--){
                    leds[j].setRGB(background.R,background.G,background.B); // off
                    //Serial.print("/");
                    oncount = 0;
                  }
                }
                else{
                  oncount = 0;
                }
              }
              bit++; // increment if not tilda
            }
            if(bit > 23){ // if next bit is out of range
              bit = 0; // reset index
              readFile.read(&data, 3); // read next 3 bytes
              countreads++;
            }

          } // done reading and displaying a column
          bit += dummybits;
          if(bit > 23){ // if next bit is out of range
            bit -= 24; // reset index
            readFile.read(&data, 3); // read next 3 bytes
            countreads++;
          }
          int del = framedelay - (millis() - time_ref2);
          /*Serial.print("framedelay = "); Serial.println(framedelay);
          Serial.print("time elapsed / column"); Serial.println(millis() - time_ref2);
          Serial.print("del ="); Serial.println(del);
          Serial.print("reads ="); Serial.println(countreads);*/
          Serial.println();
          if(del>0){
            delay(del);
          }

          FastLED.show(); // done displaying one column of the letter
          /* this code should be useless now... TODO
          Serial.println();
          if(bit + dummybits > 23){
            bit = bit - 24 + dummybits; // reset to first non dummybit
            readFile.read(&data, 3); // read next 24 bits
          }
          */

        }// done all columns
        //update current size and time (after each letter)
        //Serial.print("\n Done letter "); Serial.println(String(c));
        unsigned long time_elapsed = millis() - time_ref;
        unsigned int in = total_col_num * INPERPIX;
        printNums(time_elapsed/1000,display_time, in/12, in%12, time_elapsed/(display_time*1000) );
        readFile.close(); // close file for that letter
        delay(1); // determine delay between letters, should be 0 since spaces are a letter...
      }// end of all letters
    }
    stop = true;
    blacklist = true;     // once done displaying will manually change sw1
    menu_index = TEXTFONT; // and go back to set time menu
    ignoreSW1 = false;
    sw1 = true; // this will go back to TEXTFONT (see readyRunText())
}
//#endregion

//#region Functions
void readySetQueue(){

}
void setQueue(){

}

void readySetTimes(){
  setEncToVal(enc2, MAXBLINK, 1000); //default to 1 sec
  setEncToVal(enc2, MAXBLINK, 1000);
}
void setTimes(){
  //enc2 and enc3 set on and off time (with click for fast adjust)
  //enc1 does menu AND full color vs shaped
}

//#endregion

#endif
