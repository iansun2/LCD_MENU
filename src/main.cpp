#include <Arduino.h>
//#include "lcd_menu.cpp"

void setup(){
    pinMode(13,OUTPUT);
    byte ttt = 90;
}

void loop(){
    digitalWrite(13,0);
    delay(1000);
    digitalWrite(13,1);
    delay(1000);
}