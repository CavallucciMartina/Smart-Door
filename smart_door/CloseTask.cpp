#include "CloseTask.h"
#include "Config.h"

#define CLOSE_DEG 0

CloseTask::CloseTask(Token *token, Servo servo){
  this->token = token;
  this->servo = servo;
}
  
void CloseTask::init(int period){
  Task::init(period);
  state = CLOSING;
  doorClosingTime = 0;
}
  
void CloseTask::tick(){
  if (token->getState() == CLOSE_STATE) {
    switch (state){
    
      case CLOSING: {
        doorClosingTime += myPeriod;
        if (token->getState() == CLOSE_STATE) {
          servo.attach(SERVO_PIN);
          servo.write(CLOSE_DEG); 
          delay(500); //Time to wait the servo to move from 175 to 0 degree
          servo.detach();
        }
        if (doorClosingTime >= DOOR_DURATION){
          doorClosingTime =  0;
          token->setState(AUTENTICATION_STATE); 
        }
        break;
      }
    } 
  }
}


