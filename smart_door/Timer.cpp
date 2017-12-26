#include "Timer.h"
#include "Arduino.h"
#include <FlexiTimer2.h>

volatile bool timerFlag;

void setFlag() {
  timerFlag = true;
}

Timer::Timer(){
  timerFlag = false;  
}


/* period in ms */
void Timer::setupPeriod(int period){
  FlexiTimer2::set(period, setFlag);
  FlexiTimer2::start();
}

void Timer::waitForNextTick(){
  /* wait for timer signal */
  while (!timerFlag){}
  timerFlag = false;  
}
