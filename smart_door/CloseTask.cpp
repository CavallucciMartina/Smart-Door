#include "CloseTask.h"
#include "Config.h"
#include "Logger.h"

#define CLOSE_DEG 0

CloseTask::CloseTask(Token *token, Servo servo){
  this->token = token;
  this->servo = servo;
}
  
void CloseTask::init(int period){
  Task::init(period);
  state = CLOSING;
  doorClosingTime = 0;
  Logger.log("CT:INIT");
}
  
void CloseTask::tick(){
  if (token->getState() == CLOSE_STATE) {
    switch (state){
    
      case CLOSING: {
        doorClosingTime += myPeriod;
        if (token->getState() == CLOSE_STATE) {
          /*
          * For the moment the engine is broken.
          */
          /*servo.write(CLOSE_DEG);*/
        }
        if (doorClosingTime >= DOOR_DURATION){
          token->setState(AUTENTICATION_STATE); 
        }
        break;
      }
    } 
  }
}


