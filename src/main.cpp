#include <Arduino.h>
#include <Definitions.h>
#include <Encoders.h>
#include <My_LCD.h>
#include <avr/pgmspace.h>
#include <Menus.h>

m current;

void setup() {
  #define INDICATOR_PIN 10
  pinMode(INDICATOR_PIN, OUTPUT);
  digitalWrite(INDICATOR_PIN, HIGH);
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.home(); lcd.clear();
  lcd.print(" Pixelstick 4.0");
  lcd.setCursor(0, 1);
  lcd.print("  (Hopefully)  ");
  lcd.noCursor();
  lcd.noBlink();

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  stickOff();

  setEncoderPins();

  Serial.begin(9600);
  //while (!Serial) {;} // wait for serial to connect
  delay(200);
  Serial.print(F("\n\nsInitializing SD card..."));
  if (!SD.begin(SDCARD_SS_PIN)) {
    Serial.println(F("initialization failed!"));
    return;
  }
  Serial.println(F("initialization done."));
  delay(100);
  current = menu_table[1];
  lcd.clear();
  menu_index=1;
  readMenuFile();
}


void loop() {
    digitalWrite(INDICATOR_PIN, LOW);
    delay(20);

    move = setLCDCursors();
    Serial.print("in menu: "); Serial.print(menu_index);
    Serial.print("   move = "); Serial.println(move);

    current.functions[0](); // current function runs continually

    if(sw1){
      if(ignoreSW1){
        ignoreSW1 = false; Serial.println("ignored SW1 hit");
      }
      else if(blacklist){
        current = menu_table[menu_index]; Serial.println("in blacklist!");
        blacklist = false;
      }
      /* if going o pare                  if parent == selected        */
      else if(menu_table[menu_index].connections[0] == menu_table[menu_index].connections[move+1]){
        current.functions[2](); // transitionBack function
        delay(400);
        menu_index = menu_table[menu_index].connections[0]; // move to parent
        current = menu_table[menu_index];
      }
      else{
        menu_index = menu_table[menu_index].connections[move+1]; // move to selected
        current = menu_table[menu_index];
        current.functions[1](); // transitionHere function
      }
      sw1=false;
      //Serial.print("sw1 hit moving to menu index: "); Serial.println(menu_index);
      //delay(2000);
      readMenuFile();
    }
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(20);
}
