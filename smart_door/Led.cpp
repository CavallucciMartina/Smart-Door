#include "Led.h"
#include "Arduino.h"

Led::Led(int pin){
  this->pin = pin;
  this->ledStatus = false;
  pinMode(pin,OUTPUT);
}

void Led::switchOn(){
  digitalWrite(pin,HIGH);
  this->ledStatus = true;
}

void Led::switchOff(){
  digitalWrite(pin,LOW);
  this->ledStatus = false;
}

bool Led::getStatus() {
  return this->ledStatus;
}
