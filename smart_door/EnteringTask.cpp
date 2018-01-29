#include "EnteringTask.h"
#include "Presence.h"
#include "Config.h"
#include "MsgService.h"

#define OPEN_DEG 175 // Not 180 degrees becouse servo could go over the end of its run
#define CLOSE_DEG 0
#define MSG_OPEN_OK "OPEN: OK"
#define MSG_OPEN_KO "OPEN: KO - TIME EXPIRED"

EnteringTask::EnteringTask(Token *token, MsgBtService *msgBtService, Servo servo){
  this->token = token;
  this->msgBtService = msgBtService;
  pir = new PassiveInfraRed(PIR_PIN);
  this->servo = servo;
}
  
void EnteringTask::init(int period){
  Task::init(period);
  state = OPENING;
  doorOpeningTime = 0;
  arrivalTime = 0;
}
  
void EnteringTask::tick(){
  if (token->getState() == OPEN_STATE) {
    switch (state){
    
      case OPENING: {
        doorOpeningTime += myPeriod;
        arrivalTime += myPeriod;
        servo.attach(SERVO_PIN);
        servo.write(OPEN_DEG);
        delay(500); // Time to wait the servo to move from 0 to 175 degrees
        servo.detach();
        if (doorOpeningTime >= DOOR_DURATION){
          doorOpeningTime =  0;
          state = OPEN;
        }
        break;
      }
    
      case OPEN: {
        arrivalTime += myPeriod;
        if (arrivalTime < MAX_DELAY && pir->isPresent()){
          doorOpeningTime = 0;
          arrivalTime = 0;
          state = OPENING;
          token->setState(WORKING_STATE); 
          MsgService.sendMsg(MSG_OPEN_OK);
          msgBtService->sendMsg(Msg(MSG_OPEN_OK));
        } else if (arrivalTime >= MAX_DELAY){
          doorOpeningTime = 0;
          arrivalTime = 0;
          state = OPENING;
          token->setState(CLOSE_STATE); 
          MsgService.sendMsg(MSG_OPEN_KO);
          msgBtService->sendMsg(Msg(MSG_OPEN_KO));
        }
        break;
      }
    } 
  }
}


