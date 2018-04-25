#ifndef MY_LCD
#define MY_LCD
#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);
char buff[BUFF_SIZE]   = {'a','b','c','d','e','f','g','h','2','z','z','z','z','z','z','z','3','z','z','z','z','z','z','z'};
short curs_locations[] = {0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
// save up to 5 numbers (# of numbers, no.1 row, no.1 col, no.1 size, no.2 row, no.2 col, no.2 size, etc. )
short num_locations[]  = {0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void printSpaces(short x){
  while(x>0){
    lcd.print(" ");
    x--;
  }
}
short numLen(int x){
  short tmp=1;
  while(x/10 != 0){ x/=10; tmp++;}
  return tmp;
}
short numLen(unsigned long x){
  short tmp=1;
  while(x/10 != 0){ x/=10; tmp++;}
  return tmp;
}

void outputBuffToLCD1(){
  lcd.clear();
  for(int i=0; i<LCD_WIDTH;i++){
    lcd.print(buff[i]);
  }
}
void outputBuffToLCD2(){
  lcd.setCursor(0,1);
  for(int i=0; i<LCD_WIDTH;i++){
    lcd.print(buff[i]);
  }
}

void printNums(int a){
  lcd.setCursor(num_locations[2],num_locations[1]);
  if(a<0){
    lcd.print("nu");
    printSpaces(num_locations[3]-2);
  }
  else{
    lcd.print(a);
    /*Serial.print("num is: ");
    Serial.print(a);
    Serial.print(" max digits is: ");
    Serial.print(num_locations[3]);
    Serial.print("  numLen returns: ");
    Serial.println(numLen(a));//*/
    printSpaces(num_locations[3]-numLen(a));
  }
}
void printNumsSkipFirst(int b){
  lcd.setCursor(num_locations[5],num_locations[4]);
  lcd.print(b);
  printSpaces(num_locations[6]-numLen(b));
}
void printNums(int a, int b){
  lcd.setCursor(num_locations[2],num_locations[1]);
  if(a<0){
    lcd.print("nu");
    printSpaces(num_locations[3]-2);
  }
  else{
  lcd.print(a);
  printSpaces(num_locations[3]-numLen(a));
  }
  lcd.setCursor(num_locations[5],num_locations[4]);
  lcd.print(b);
  printSpaces(num_locations[6]-numLen(b));
}
void printNumsTwoNu(int a, int b){
  lcd.setCursor(num_locations[2],num_locations[1]);
  if(a<0){
    lcd.print("nu");
    printSpaces(num_locations[3]-2);
  }
  else{
    lcd.print(a);
    printSpaces(num_locations[3]-numLen(a));
  }

  lcd.setCursor(num_locations[5],num_locations[4]);
  if(b<0){
    lcd.print("nu");
    printSpaces(num_locations[3]-2);
  }
  else{
    lcd.print(b);
    printSpaces(num_locations[3]-numLen(a));
  }
}
void printNums(int a, int b, int c){
  lcd.setCursor(num_locations[2],num_locations[1]);
  lcd.print(a);
  printSpaces(num_locations[3]-numLen(a));

  lcd.setCursor(num_locations[5],num_locations[4]);
  lcd.print(b);
  printSpaces(num_locations[6]-numLen(b));

  lcd.setCursor(num_locations[8],num_locations[7]);
  lcd.print(c);
  printSpaces(num_locations[9]-numLen(c));
}
void printNums(int a, int b, int c, int d){
  lcd.setCursor(num_locations[2],num_locations[1]);
  lcd.print(a);
  printSpaces(num_locations[3]-numLen(a));

  lcd.setCursor(num_locations[5],num_locations[4]);
  lcd.print(b);
  printSpaces(num_locations[6]-numLen(b));

  lcd.setCursor(num_locations[8],num_locations[7]);
  lcd.print(c);
  printSpaces(num_locations[9]-numLen(c));

  lcd.setCursor(num_locations[11],num_locations[10]);
  lcd.print(d);
  printSpaces(num_locations[12]-numLen(c));
}
void printNums(int a, int b, int c, int d, int e){
  lcd.setCursor(num_locations[2],num_locations[1]);
  lcd.print(a);
  printSpaces(num_locations[3]-numLen(a));

  lcd.setCursor(num_locations[5],num_locations[4]);
  lcd.print(b);
  printSpaces(num_locations[6]-numLen(b));

  lcd.setCursor(num_locations[8],num_locations[7]);
  lcd.print(c);
  printSpaces(num_locations[9]-numLen(c));

  lcd.setCursor(num_locations[11],num_locations[10]);
  lcd.print(d);
  printSpaces(num_locations[12]-numLen(c));

  lcd.setCursor(num_locations[14],num_locations[13]);
  lcd.print(d);
  printSpaces(num_locations[15]-numLen(c));
}


#endif
