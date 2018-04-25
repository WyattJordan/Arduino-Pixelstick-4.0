#ifndef Encoders
#define Encoders
#include <Arduino.h>
#include <EnableInterrupt.h>
// declaration of encoder variables, encx, swcx, and swx
volatile unsigned short enc1 = 33002;
volatile unsigned short enc2 = 33000;
volatile unsigned short enc3 = 33000;
volatile unsigned short swc1 = 0;
volatile unsigned short swc2 = 0;
volatile unsigned short swc3 = 0;
bool enc1_changed = false;
bool enc2_changed = false;
bool enc3_changed = false;
bool sw1 = false;
bool sw2 = false;
bool sw3 = false;



void interruptonD0(){   //D3 is clock, D0 is data on encoder 1
    enc1_changed = true;
    if(digitalRead(1))
      enc1--;
    else
      enc1++;


}
void interruptonD4(){   //D5 is clock, D5 is data on encoder 2
    enc2_changed = true;
    if(digitalRead(5))
      enc2++;
    else
      enc2--;

}
void interruptonD6(){   //D7 is clock, D6 is data on encoder 3
  enc3_changed = true;
  if(digitalRead(7))
    enc3--;
  else
    enc3++;

}
void interruptonA1(){   //encoder 1 button
  sw1=true;swc1++;
}
void interruptonA2(){   //encoder 2 button
  sw2=true;swc2++;}
void interruptonD8(){   //encoder 3 button
  sw3=true;
  swc3++;
}
void setEncoderPins(){
  //enc 1
  pinMode(0, INPUT);   //enc1 DT
  pinMode(1, INPUT);   //enc1 CLK
  pinMode(A1, INPUT);  //enc1 switch
  enableInterrupt(0,interruptonD0,RISING);
  enableInterrupt(A1,interruptonA1,RISING); //switch
  //enc 2
  pinMode(4, INPUT);   //enc2 DT
  pinMode(5, INPUT);   //enc2 CLK
  pinMode(A2, INPUT);  //enc2 switch
  enableInterrupt(4,interruptonD4,RISING);
  enableInterrupt(A2,interruptonA2,RISING);
  //enc3
  pinMode(6, INPUT);   //enc3 DT
  pinMode(7, INPUT);   //enc3 CLK
  pinMode(8, INPUT);   //enc3 switch
  enableInterrupt(6,interruptonD6,RISING);
  enableInterrupt(8,interruptonD8,RISING); //switch
}
void setEncTo0(volatile unsigned short &enc, short size){
  enc += size - enc%size;
}

void setEncToVal(volatile unsigned short &enc, short size, short val){
  if(size!=0){
    enc += size - enc%size + val;
  }
}

#endif
