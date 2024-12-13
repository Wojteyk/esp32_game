#include <Arduino.h>
#include <TFT_eSPI.h> 
#include <SPI.h>
#include "header.h"

#define xPin 33
#define yPin 32
#define swPin 19

void setup() {

  //pin settings
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(swPin, INPUT);

  Serial.begin(9600);
  initScreen();
  
}

void loop() {
  //changing screens
  screen();
}


