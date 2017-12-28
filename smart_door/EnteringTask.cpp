#include "EnteringTask.h"
#include "Presence.h"
#include "config.h"
#include "MsgService.h"
#include "Logger.h"

#define OPEN_DEG 165
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
  servo.write(CLOSE_DEG);
  delay(50);
  Logger.log("ET:INIT");
}
  
void EnteringTask::tick(){
  if (token->getState() == OPEN_STATE) {
    switch (state){
    
      case OPENING: {
        doorOpeningTime += myPeriod;
        arrivalTime += myPeriod;
        servo.write(OPEN_DEG);
        if (doorOpeningTime >= DOOR_DURATION){
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


