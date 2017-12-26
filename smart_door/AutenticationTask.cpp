#include "AutenticationTask.h"
#include "Sonar.h"
#include "config.h"
#include "MsgService.h"
#include "Logger.h"

#define MSG_USERNAME "USR: "
#define MSG_PASSWORD "PWD: "
#define MSG_DISTANCE "DIST: "
#define MSG_VALID "VALID: OK"
#define MSG_INVALID "VALID: KO"
#define THIS_TOKEN "TOKEN_AUTENTICATION"
#define TOKEN_OPEN "TOKEN_OPENING"
#define MSG_WELCOME "WELCOME"
#define MSG_DENIED "RETRY"

AutenticationTask::AutenticationTask(Token *token, MsgBtService *msgBtService){
  this->token = token;
  this->msgBtService = msgBtService;
  prox = new Sonar(PROX_ECHO_PIN,PROX_TRIG_PIN);
  ledOn = new Led(LED_CONNECTED_PIN);
  
}
  
void AutenticationTask::init(int period){
  Task::init(period);
  state = IDLE;
  ledOn->switchOn();
  Logger.log("AT:INIT");
}
  
void AutenticationTask::tick(){
  if (token->getState() == THIS_TOKEN) {
    switch (state){
    
      case IDLE: {
       if (msgBtService->isMsgAvailable()){
         state = USR;
         Msg* msg = msgBtService->receiveMsg(); 
         const String& usr = msg->getContent();
         MsgService.sendMsg(MSG_USERNAME + usr);
         delete msg;
        } else {
          double value = prox->getDistance();
          msgBtService->sendMsg(Msg(MSG_DISTANCE + String(value)));
        }
        break;      
      }
    
      case USR: {
        if (msgBtService->isMsgAvailable()){
          state = PWD;
          Msg* msg = msgBtService->receiveMsg(); 
          const String& pwd = msg->getContent();
          MsgService.sendMsg(MSG_PASSWORD + pwd);
          delete msg;
        } 
        break;
      }
    
      case PWD: {
        if (MsgService.isMsgAvailable()){
          Msg* msg = MsgService.receiveMsg(); 
          const String& validation = msg->getContent();
          if (validation.equals(MSG_VALID)) {
            msgBtService->sendMsg(Msg(MSG_WELCOME));
            token->setState(TOKEN_OPEN);
            state = IDLE;
          } else if (validation.equals(MSG_INVALID)) {
            msgBtService->sendMsg(Msg(MSG_DENIED));
            state = IDLE;
          }
          delete msg;
          break;
        }
      }
    } 
  }
}


