#ifndef TEXTFUNCTIONS
#define TEXTFUNCTIONS
#include <GeneralFunctions.h>
short num_strings, string_len, string_index, font_index, height_index, pixel_height,
display_time, back_width; //will also use start for offsetting text
bool bold, italics;
unsigned long string_pixel_width, string_in_width;

unsigned short type, charval, letter_index;
char types[] = {'A', 'a', '0', '!'};
/* 39 is '  34 is "  92 is \    total length is 32*/
char syms[] = { ' ','!','?','@','*',',','.',39,34,'#','<','=','>','+','$','%','&',
'-',':',';','^','_','`','/',92,'{','|','}','[',']','(',')','~'};
char string[STRING_SIZE]   = {'a','b','c','d','e','f','g','h','i','z','z','z',
'z','z','z','z','a','b','c','d','e','f','g','h','i','z','z','z','z','z','z',
'z','a','b','c','d','e','f','g','h','i','z','z','z'};
short height[] = {72,144,216,288};
// different available fonts, 11 char max for display
const short num_fonts = 4;
const char string_0[] PROGMEM = "Agency";
const char string_1[] PROGMEM = "ComicSans";
const char string_2[] PROGMEM = "SansSerif";
const char string_3[] PROGMEM = "TimesRoman";
char font_buff[] = {'`','`','`','`','`','`','`','`','`','`','`','`','`'}; //13 for terminator
const char *font_table[] PROGMEM =	   // change "string_table" name to suit
{
  string_0,
  string_1,
  string_2,
  string_3};

void clearString(){
  for(int i=0; i<BUFF_SIZE; i++){
    string[i] = '`';
  }
}

void printString(int start2, bool editing){
  lcd.home();
  if(editing && letter_index == string_len && letter_index<STRING_SIZE-1){ start2++;}

  if(string_len > LCD_WIDTH -1){// if the string is longer than the LCD
    for(int i=start2; i<num_locations[3]+start2;i++){
      if(editing && i==letter_index){
        lcd.print(String((char) charval));
      }
      else{
        lcd.print(string[i]);
      }
    }
  }
  else{// if the string is 16 chars or less
    for(int i=0; i<num_locations[3];i++){
      if(editing && i==letter_index){
        lcd.print(String((char) charval));
      }
      else{
        lcd.print(string[i]);
      }
    }
  }
}// end of printString() function


void printStringEditing(){///////////////////////////////////////////////////////////////////
  printString(start, true);
  lcd.setCursor(num_locations[2] + letter_index - start + 1, 0/* first row*/);
  lcd.print((char) charval);
  lcd.setCursor(num_locations[5],num_locations[4]);
  lcd.print("A");
  lcd.setCursor(num_locations[5]+type,num_locations[4]); // place at char type
}


void printFontName(){
  lcd.setCursor(num_locations[2],num_locations[1]);
  printSpaces(12); // wipe it cause different font names
  lcd.setCursor(num_locations[2],num_locations[1]);
  lcd.print(font_table[font_index]);

  lcd.setCursor(num_locations[5],num_locations[4]);

  lcd.print(height[height_index]);
  printSpaces(num_locations[6] - numLen(height[height_index]));

  lcd.setCursor(num_locations[8],num_locations[7]);
  if(bold){lcd.print("Y");}
  else{lcd.print("N");}
  lcd.setCursor(num_locations[11],num_locations[10]);
  if(italics){lcd.print("Y");}
  else{lcd.print("N");}
}
short getNumSDStrings(){
  int count = 0; String file;
  file = "strings/str0.txt";
  while(SD.exists(file.c_str())){
    Serial.println(file + " exists");
    count++;
    file = "strings/str" + String(count) + ".txt";
  }
  if(count>100){  //LCD designed for only 2 digits, also 100 wands is plenty
    Serial.println("num_strings > 100, returning 99");
    return 99;
  }
  Serial.print("num_strings = "); Serial.println(count);
  return count;//*/
}
void printStringIndex(){
  lcd.setCursor(num_locations[5],num_locations[4]);
  lcd.print(string_index);
  printSpaces(num_locations[6]-numLen(string_index));
}
void writeStringFile(){
  String file;
  if(bwand_num==-1){
    file = "strings/str" + String(getNumBWands(false)) + ".txt";
  }
  else{
    file = "strings/str" + String(bwand_num) + ".txt";
    SD.remove(file.c_str());
  }
  f = SD.open(file, FILE_WRITE);
  f.println(string_len);
  for(int i=0; i<string_len; i++){
    f.print(string[i]);
  }
  f.close();
}
void readSDString(){
  clearString();
  String temp = String(string_index);
  String file = "strings/str" + temp + ".txt";

  if(string_index<0){
    string_len = 2;
    string[0] = 'n'; string[1] = 'u';
  }
  else if(SD.exists(file.c_str())){ // first line is string length, second is actual string
    ifstream myFileStream;
    myFileStream.open(file.c_str());
    string_len = readLineGetNum(myFileStream);
    myFileStream.getline(string, STRING_SIZE, '\n');
    string[string_len]='`';
    myFileStream.close();
  }
}
String getFontDir(){
  String temp = "r";
  if(bold && !italics){
    temp = "b";
  }
  else if(italics && !bold){
    temp = "i";
  }
  else if( bold && italics){
    temp = "c";
  }

  Serial.println("about to convert char to String");
  return "f/" + String(font_table[font_index]) + "/" + String(height[height_index]) + temp + "/";

}
void getTextLength(){
  string_pixel_width=0;
  String dir = getFontDir();
  String file = dir + "sizes.txt";
  if(SD.exists(file.c_str())){
    ifstream myFileStream;
    myFileStream.open(file.c_str());
    pixel_height = readLineGetNum(myFileStream);
    Serial.print("pixel_height = "); Serial.println(pixel_height);
    myFileStream.close();
    //Serial.println("entering string[i] loop");
    for(int i=0; i< string_len; i++){
      ifstream myFileStream;
      myFileStream.open(file.c_str());
      myFileStream.getline(buff, LCD_WIDTH+1, '\n');
      char c = string[i];
      if(c == ' '){c = 'A';} // spaces don't exist, use width of cap A
      while(true){
        myFileStream.getline(buff, LCD_WIDTH+1, '\n');
        if(buff[0]==c){
          break;
        }
      }
      buff[0] = buff[1]; // re-arrange since first char is the char to display
      buff[1] = buff[2]; // just want the numbers after the char
      buff[2] = buff[3];
      buff[3] = 'z';
      string_pixel_width += getBuffNum();
      myFileStream.close();
    } // end of looping thru all characters in string
  }// end of reading file
  string_in_width = string_pixel_width/3.6585; // using 144px/meter conversion
  /*Serial.print("pixel width = "); Serial.println(string_pixel_width);
  Serial.print("in width = "); Serial.println(string_in_width);//*/
}

void displayFontSkeleton(){
  for(int i=0; i<NUM_LEDS; i++){
    if(i<start-back_width || i>start+pixel_height+back_width){
      leds[i].setRGB(0,0,0); // off
    }
    else{
      if(i>=start && i<=start+pixel_height){
        leds[i].setRGB(R,G,B); // text color
      }
      else{
        leds[i].setRGB(background.R,background.G,background.B); //backgrond color
      }
    }
  }
  FastLED.show();
}
void displayTwoColors(){
  for(int i=0; i<NUM_LEDS/2;i++){
    leds[i].setRGB(R,G,B); // text color
  }
  for(int i=NUM_LEDS/2; i<NUM_LEDS;i++){
    leds[i].setRGB(background.R,background.G,background.B); //backgrond color
  }
  FastLED.show();
}


//not used anymore
void copyBuff2String(){
  int i=0;
  for(i; i<BUFF_SIZE; i++){
    if(buff[i] != '`'){
        string[i] = buff[i];
    }
    else{
      string[i] = '`';
    }
  }
}
#endif
