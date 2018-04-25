#ifndef GENERALFUNCTIONS
#define GENERALFUNCTIONS
//#region includes
#include <Arduino.h>
#include <SPI.h>
#include "SdFat.h"
#include <stdio.h>
#include <Wire.h>

#include <Definitions.h>
#include <Encoders.h>
#include <My_LCD.h>

#include "FastLED.h"
//#endregion

//#region Global Variables (BSS)
CRGB leds[NUM_LEDS];	//declare the LED array

struct color{
  short R,G,B;
};
color * bandcols;
color background;
char menu_type = 'm';
short menu_index = 1;
File f;
SdFat SD;

unsigned char R,G,B,H,S,V;
unsigned short start, len, num_wands, num_band_files,
num_bands, num_iters, bandcol;
unsigned long time_ref, time_ref2;
float framedelay;
short bwand_num, band_blend, bwand_num2;

int move=-1;
bool ignoreSW1 = false;
bool blacklist = false;
//#endregion

int setLCDCursors(){// puts the ">" in the correct spot and erases all other previous cursors
  if(curs_locations[0]==0){
    return -1;
  }
  else{
    int pos = enc1%curs_locations[0];
    for(int i = 1; i<curs_locations[0]*2; i+=2){
      lcd.setCursor(curs_locations[i+1],curs_locations[i]);
      if(pos == (i+1)/2 - 1){ //enc1 is selecting this cursor
        if(curs_locations[i+1]==15){lcd.print("^");} //if at very end make carrot
        else{lcd.print(F(">"));}
      }
      else{
        lcd.print(F(" "));
      }
    }//*/
  return pos;
  }
}
void outputBuff(bool line){
  for(int i = 0; i<BUFF_SIZE; i++){
    Serial.print(buff[i]);
  }
  if(line){
    Serial.print('\n');
  }
}
void clearBuff(){
  for(int i=0; i<BUFF_SIZE; i++){
    buff[i] = '`';
  }
}
void padArray(short x[], int start, int val){// pads array from start to end with val
  for(int i= start; i<ARRAY_LENGTH; i++){
    x[i] = val;
  }
}
int getBuffNum(){//reads buff[] for a number and returns that number
  int i=0;
  String tmp = "";
  while(buff[i]>47 && buff[i]<58){
    tmp += buff[i++];
  }
  return atoi(tmp.c_str());
}
int readLineGetNum(ifstream &myFileStream){
  clearBuff();
  myFileStream.getline(buff, 8, '\n');
  //outputBuffToSerial();
  return getBuffNum();
}
void readMenuFile(){//reads + saves the numbers stored in the file and writes to the LCD
  //Serial.println("\nIn displayMenu");
  ifstream myFileStream;
  String menufile = "m/" + String(menu_index) + ".txt";
  //Serial.println(menufile);
  myFileStream.open(menufile.c_str());
  //Serial.println("opened file");
  myFileStream.getline(buff, LCD_WIDTH+1, '\n');
  curs_locations[0] = getBuffNum();
  int i = 1;
  for(i; i<curs_locations[0]*2+1; i++){
    myFileStream.getline(buff, LCD_WIDTH+1, '\n');
    curs_locations[i] = getBuffNum();
  }
  padArray(curs_locations, i, -1);

  myFileStream.getline(buff, LCD_WIDTH+1, '\n');
  num_locations[0] = getBuffNum();
  i = 1;
  for(i; i<num_locations[0]*3+1; i++){
    myFileStream.getline(buff, LCD_WIDTH+1, '\n');
    num_locations[i] = getBuffNum();
    }
  padArray(num_locations, i, -1);
  //*/
  //Serial.println("read nums");
  //actually output the last 2 lines to lcd
  myFileStream.getline(buff, LCD_WIDTH+1, '\n');
  outputBuffToLCD1();
  myFileStream.getline(buff, LCD_WIDTH+1, '\n');
  outputBuffToLCD2();
  myFileStream.close();
  enc1+= curs_locations[0] - enc1%curs_locations[0]; //set enc1 to 0 (first menu option)
}


short getNumBWands(bool bands){ // move to setup? needs the SD to have begun?
  int count = 0; String file;
  if(bands){file = "bands/band0.txt";}
  else{file = "wands/wand0.txt";}
  Serial.println("looking for " + file);
  while(SD.exists(file.c_str())){
    Serial.println(file + " exists");
    count++;
    if(bands){file = "bands/band" + String(count) + ".txt";}
    else{file = "wands/wand" + String(count) + ".txt";}
  }
  if(count>100){  //LCD designed for only 2 digits, also 100 wands is plenty
    return 99;
  }
  return count;//*/
}
void setEncByClick(short swcount, short range, volatile unsigned short &enc, bool &sw ){
  if(sw){
    sw=false;
    if     (swcount%4==0){setEncToVal(enc, range, range/4);}
    else if(swcount%3==0){setEncToVal(enc, range, range/2);}
    else if(swcount%2==0){setEncToVal(enc, range, range*3/4);}
    else if(swcount%2==1){setEncToVal(enc, range, range-1);}
  }
}
void setValuesHSV(){
  H = enc1%MYSCALE;
  S = enc2%MYSCALE;
  V = enc3%MYSCALE;
}
void setValuesRGB(){
  R = enc1%MYSCALE;
  G = enc2%MYSCALE;
  B = enc3%MYSCALE;
}
void setSW2ValuesRGB(){
  int tmp = swc2%3;
  if(sw2){
    sw2=false;
    if(tmp==0){
      enc2 = MYSCALE*100+R;
    }
    else if(tmp==1){
      enc2 = MYSCALE*100+G;
    }
    else{
      enc2 = MYSCALE*100+B;
    }
  }
  int val = enc2%MYSCALE;
  if(tmp==0){R = val;}
  else if(tmp == 1){G = val;}
  else{B = val;}
}
void setSW2ValuesHSV(){
  int tmp = swc2%3;
  if(sw2){
    sw2=false;
    if(tmp==0){
      enc2 = MYSCALE*100+H;
    }
    else if(tmp==1){
      enc2 = MYSCALE*100+S;
    }
    else{
      enc2 = MYSCALE*100+V;
    }
  }
  int val = enc2%MYSCALE;
  if(tmp==0)       {H = val;}
  else if(tmp==1)  {S = val;}
  else             {V = val;}
}

void setBandColInArrayRGB(){
  bandcols[bandcol] = {R,G,B};
}

//Functions for the Brush_Functions.h file
void stickOff(){
  for(int i =0; i<NUM_LEDS; i++){
    leds[i].setRGB(0,0,0);
  }
  FastLED.show();
}
void mapRGBtoHSV(){
  // map RGB values to HSV
  Serial.println("Mapped RGB to HSV");
  float delta, temp, Hflt;
  if((R>G && R>B) || (R==G && R>B) || (R==B && R>G) ){ //R is max
  	if(G>B){delta = (float)(R-B)/255;}
    else{delta = (float)(R-G)/255;}
    if(R!=0) {S = 255.0*(delta/((float)R/255.0));} else{S=0;};
  	temp = ((float)G/255.0 - (float)B/255.0 )/ delta;
  	/*int temp2 = temp;
    Serial.print("temp ="); Serial.println(temp);
    Serial.print("temp2 ="); Serial.println(temp2);
    Serial.print("temp2mod6 ="); Serial.println(temp2%6);
    //H = 60 *(temp2%6 + temp);//*/
    Hflt = 60.0 *(temp + 6);
    V = R;
  }
  else if((G>B && G>R) || (G==B && G>R)){  //G is max
    if(R>B){delta = (float)(G - B)/255;}
    else{delta = (float)(G - R)/255;}
    if(G!=0) {S = 255.0*(delta/((float)G/255.0));} else{S=0;}
	  temp = (float) ((float)B/255.0 - (float)R/255.0 )/ delta;
    Hflt = 60 * (temp + 2);
    V = G;
  }
  else /*if(B>G && B>R)*/{  //B is max or all 3 are equal
  	if(R>G){delta = (float)(B - G)/255;}
    else{delta = (float)(B - R)/255;}
    if(B!=0) {S = 255.0*(delta/((float)B/255.0));} else{S=0;};
  	temp = ((float)R/255 - (float)G/255)/delta;
    Hflt = 60 * (temp + 4);
    V = B;
  }// if for finding max

  if(delta==0){H=0;} else{H = ( (float) Hflt/360.0 ) * 255.0;}

  //Serial.println("mapped RGB values to HSV");
}
void mapHSVtoRGB(){
  Serial.println("Mapped HSV to RGB");
  // map HSV values to RGB
  float c, x, m, r,g,b;
  c = (float) V/255.0 * (float) S/255.0;
  float H2 = H*360.0/255.0; //scale to 360deg hue wheel
  int Htemp = H2;
  H2 /= 60.0;
  int H3 = H2;
  H2 = H3%2 + H2-H3 - 1;
  if(H2<0) {H2*=-1;}
  x = c*(1-H2);
  m = (float) V/255 - c;
  if(Htemp<60){r=c; g=x; b=0;}
  else if(Htemp<120){r=x; g=c; b=0;}
  else if(Htemp<180){r=0; g=c; b=x;}
  else if(Htemp<240){r=0; g=x; b=c;}
  else if(Htemp<300){r=x; g=0; b=c;}
  else              {r=c; g=0; b=x;}
  R = (r+m)*255.0;
  G = (g+m)*255.0;
  B = (b+m)*255.0;
  //Serial.println("mapped HSV values to RGB");
}
void flashWandHSV(short del, short numflashes, unsigned char h,unsigned char s,unsigned char v){
  while(numflashes>0){
    while(start < start + len && start<NUM_LEDS){
      leds[start].setHSV(h,s,v);
    }
    FastLED.show();
    delay(del);
    stickOff();
    delay(del);
    numflashes--;
  }
}
void flashStickHSV(short del, short numflashes, unsigned char h,unsigned char s,unsigned char v){
  while(numflashes>0){
    for(int i =0; i<NUM_LEDS; i++){
      leds[start].setHSV(h,s,v);
    }
    delay(del/2);
    stickOff();
    delay(del/2);
    numflashes--;
  }
}
void flashStickRGB(short del, short numflashes, unsigned char r,unsigned char g,unsigned char b){
  while(numflashes>0){
    for(int i =0; i<NUM_LEDS; i++){
      leds[i].setRGB(r,g,b);
    }
    FastLED.show();
    delay(del);
    stickOff();
    delay(del);
    numflashes--;
  }
}
void stickRainbow(int sat, int bright){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i].setHSV(256 * (float) i/NUM_LEDS, sat,bright);
  }
  FastLED.show();
  //Serial.println("showing Rainbow");
}
//Wand Functions
void displayLocalWand(int start, int len, unsigned short R, unsigned short G, unsigned short B){
  for(int i=0; i<NUM_LEDS; i++){
    if(i<start || i>(start+len)){
      leds[i].setRGB(0,0,0);
    }
    else{
      leds[i].setRGB(R,G,B);
    }
  }
  /*while(temp < start + len && temp<NUM_LEDS){
    leds[temp].setRGB(R,G,B);
    temp++;
  }//*/
  FastLED.show();//*/
}
void displayLocalWand(int start, int len, unsigned short R, unsigned short G, unsigned short B, bool &condition1){
  if(condition1){
    condition1 = false;
    displayLocalWand(start, len, R,G,B);
  }
}
void displayLocalWand(int start, int len, unsigned short R, unsigned short G, unsigned short B, bool &condition1, bool &condition2){
  if(condition1 || condition2){
    condition1 = false;
    condition2 = false;
    displayLocalWand(start, len, R,G,B);
  }
}
bool readSDWand(short index){
  String temp = String(index);
  String file = "wands/wand" + temp + ".txt";
  if(index==-1){
    R=0; G=0; B=0;
    return true;
  }
  else if(SD.exists(file.c_str())){
    // numbers in file are: start, len, R, G, B, each on a new line
    ifstream myFileStream;
    myFileStream.open(file.c_str());
    start = readLineGetNum(myFileStream);
    len   = readLineGetNum(myFileStream);
    R = readLineGetNum(myFileStream);
    G = readLineGetNum(myFileStream);
    B = readLineGetNum(myFileStream);
    myFileStream.close();

    /*Serial.println("reading wand from SD");
     Serial.print("start ="); Serial.println(start);
    Serial.print("len ="  ); Serial.println(len);
    Serial.print("R ="); Serial.println(R);
    Serial.print("G ="); Serial.println(G);
    Serial.print("B ="); Serial.println(B);//*/
  }
  return false;
}
void displaySDWand(){
  bool temp = readSDWand(bwand_num);
  if(temp){
    stickRainbow(255,180);
  }
  else{
    displayLocalWand(start,len,R,G,B);
  }
}
void dispalySDWand(bool &condition){
  if(condition){
    displaySDWand();
  }
  condition = false;
}
void dispalySDWand(bool &condition1, bool &condition2){
  if(condition1 || condition2){
    displaySDWand();
  }
  condition1 = condition2 = false;
}
void writeWandFile(){
  String file;
  if(bwand_num==-1){
    file = "wands/wand" + String(getNumBWands(false)) + ".txt";
  }
  else{
    file = "wands/wand" + String(bwand_num) + ".txt";
    SD.remove(file.c_str());
  }
  f = SD.open(file, FILE_WRITE);
  f.println(start);
  f.println(len);
  f.println(R);
  f.println(G);
  f.println(B);
  f.close();
}

//Band Functions
short getNearestNeighborColor(short loc, short band_width, short col1){
  short temp;
  if((band_width/2) < loc){
    if(col1 + 1 > num_bands - 1){
      temp = 0;
    }
    else{
      temp = col1 + 1 ;
    }
  }
  else if((band_width/2) > loc){
    if(col1 > 0){
      temp = col1 - 1;
    }
    else{
      temp = num_bands -1;
    }
  }
  else{ // if it's dead center
    temp = col1;
  }
  return temp;
}
bool incrementUp(short col1, short col2){
  if(col1>col2){
    if(255-col1+col2<col1-col2){return true;}
    else{return false;}
  }
  else{
    if(255 - col2 + col1< col2-col1){return false;}
    else{return true;}
  }
}
void displayLocalBandEdited(){
  short band_width = NUM_LEDS/(num_iters*num_bands);
  short offset = (NUM_LEDS - num_iters*num_bands*band_width);
  short first_offset = offset/2;
  for(int i=0; i<NUM_LEDS; i++){
    if(i<first_offset || i>NUM_LEDS-1-(offset-first_offset)){leds[i].setRGB(0, 0, 0);}
    else{
        short col1 = ( (i-first_offset)/band_width ) % num_bands;
        if(col1 == bandcol){
          leds[i].setRGB(R, G, B);
        }
        else{
          leds[i].setRGB(bandcols[col1].R, bandcols[col1].G, bandcols[col1].B);
        }
      }// else !offset
    }// for
    FastLED.show();
}
void displayLocalBand(bool colors){
  unsigned long time1 = millis();
  short band_width = NUM_LEDS/(num_iters*num_bands);
  short offset = (NUM_LEDS - num_iters*num_bands*band_width);
  short first_offset = offset/2;
  if(!colors){ // display hue-separated bands
    //Serial.println("going to disp nu");
    delete[] bandcols;
    bandcols = new color[num_bands];
    for(int i=0; i<num_bands;i++){
      H = 256/(num_bands+1) * i;
      S = 255;
      V = 200;
      mapHSVtoRGB();
      color temp;
      temp.R = R;
      temp.G = G;
      temp.B = B;
      bandcols[i] = temp;
    }
  }
  int col2;
  for(int i=0; i<NUM_LEDS; i++){
    if(i<first_offset || i>NUM_LEDS-1-(offset-first_offset)){leds[i].setRGB(0, 0, 0);}
    else{
      short col1 = ( (i-first_offset)/band_width ) % num_bands;

      if(band_blend==0){
        leds[i].setRGB(bandcols[col1].R, bandcols[col1].G, bandcols[col1].B);
      }
      else{ // if blending
        if(i<band_width/2 + first_offset || i>(NUM_LEDS - 1 - (offset - first_offset) - band_width/2)){ // end colors dont fade
          leds[i].setRGB(bandcols[col1].R, bandcols[col1].G, bandcols[col1].B);
        }
        else{
          Serial.println("blending colors...");
          band_blend += 1; // range 2:100
          short band_loc = (i-first_offset) % band_width; // where in the band it is;
          short half_band = band_width/2;

          double lvl;
          if(band_loc<half_band){
            lvl = band_loc;
          }
          else{
            lvl = 2*half_band-band_loc;
          }

          col2 = getNearestNeighborColor(band_loc, band_width, col1); //what to fade to
          R = bandcols[col1].R;
          G = bandcols[col1].G;
          B = bandcols[col1].B;
          mapRGBtoHSV();
          unsigned char h,s,v;
          h = H; s = S; v = V; // copy col 1 vals to temp vals
          R = bandcols[col2].R;
          G = bandcols[col2].G;
          B = bandcols[col2].B;
          mapRGBtoHSV();      // col2 vals now in normal H,S,V vars

          signed char temp1, temp2, temp3;
          bool up = incrementUp(h,H);
          temp1 = h-H;
          if(temp1<0){temp1*=-1;} // get absolue difference, up function determines sign

          int temp_H;
          temp_H = h + ((1*up + -1*(!up)) * 1.0/(2.0+lvl)*(temp1)*band_blend/100.0 );
          if(temp_H>255){
            H = temp_H - 255;
          }
          else if (temp_H<0){
            H = temp_H + 255;
          }
          else{
            H = temp_H;
          }
          //((1*(s<S) + -1*(s>S)) *
          //((1*(h<H)) + -1*(h>H)) *

          temp2 = S-s;
          temp3 = V-v;
          S = s +  (short)  1.0/(2.0+lvl)*(temp2)*band_blend/100.0;
          V = v +  (short)  1.0/(2.0+lvl)*(temp3)*band_blend/100.0;
          leds[i].setHSV(H,S,V);
        }
      }// if blending
    }// if not outside offset
  }// for loop
  FastLED.show();
  time1 = millis() - time1;
  Serial.print("time to disp band is: ");
  Serial.println(time1);
}
void displaySDBand(){
  String temp = String(bwand_num);
  String file = "bands/band" + temp + ".txt";
  if(bwand_num==-1){
    //Serial.println("bwand_num == -1 for nu");
    displayLocalWand(134,20,255,0,0);
  }
  else if(SD.exists(file.c_str())){
    ifstream myFileStream;
    myFileStream.open(file.c_str());
    num_iters = readLineGetNum(myFileStream);
    num_bands = readLineGetNum(myFileStream);
    delete[] bandcols;
    bandcols = new color[num_bands];
    //Serial.println("made new memory");
    for(int i=0; i<num_bands;i++){
      bandcols[i].R = readLineGetNum(myFileStream);
      bandcols[i].G = readLineGetNum(myFileStream);
      bandcols[i].B = readLineGetNum(myFileStream);
    }
    myFileStream.close();
    displayLocalBand(true);

  }// if
} // displaySDBand()
void dispalySDBand(bool &condition){
  if(condition){
    displaySDBand();
  }
  condition = false;
}
void dispalySDBand( bool &condition1, bool &condition2){
  if(condition1 || condition2){
    displaySDBand();
  }
  condition1 = condition2 = false;
}
void writeBandFile(){
  Serial.println("writing band file");
  String file;
  if(bwand_num==-1){
    file = "bands/band" + String(getNumBWands(true)) + ".txt";
  }
  else{
    file = "bands/band" + String(bwand_num) + ".txt";
    SD.remove(file.c_str());
  }
  f = SD.open(file, FILE_WRITE);
  f.println(num_iters);
  f.println(num_bands);
  for(int i=0; i<num_bands;i++){
    f.println(bandcols[i].R);
    f.println(bandcols[i].G);
    f.println(bandcols[i].B);
  }
  f.close();
}

#endif
