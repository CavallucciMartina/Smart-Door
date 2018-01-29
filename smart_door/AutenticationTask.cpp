#include "AutenticationTask.h"
#include "Sonar.h"
#include "Config.h"
#include "MsgService.h"
//#include "Logger.h"

#define MSG_ASK_DIST "DIST?"
#define MSG_USERNAME "USR: "
#define MSG_PASSWORD "PWD: "
#define MSG_DISTANCE "DIST: "
#define MSG_VALID "VALID: OK"
#define MSG_INVALID "VALID: KO"
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
  //Logger.log("AT:INIT");
}
  
void AutenticationTask::tick(){
  if (token->getState() == AUTENTICATION_STATE) {
    switch (state){
    
      case IDLE: {
       if (msgBtService->isMsgAvailable()){
        delay(10); //Time to wait to allow the bluetooth module to receive correctly the whole message
        Msg* msg = msgBtService->receiveMsg();
        const String& data = msg->getContent(); 
        if (data.startsWith(MSG_USERNAME)){
          state = USR;
          MsgService.sendMsg(data);
        } else if (data == MSG_ASK_DIST) {
          /*
         * Call sendMsg on a MsgBtService istance cause an interference with Servo.h if it's called frequently.
         * Servo probably would breake because it moves back and forward continuosly in un unusual way.
         */
         double value = prox->getDistance();
         msgBtService->sendMsg(Msg(MSG_DISTANCE + String(value)));
        }
        delete msg;
       }
       break;      
      }
    
      case USR: {
        if (msgBtService->isMsgAvailable()){
          delay(10); //Time to wait to allow the bluetooth module to receive correctly the whole message
          state = PWD;
          Msg* msg = msgBtService->receiveMsg(); 
          const String& data = msg->getContent();
          if (data.startsWith(MSG_PASSWORD)) {
            MsgService.sendMsg(data);
          }
          delete msg;
        } 
        break;
      }
    
      case PWD: {
        if (MsgService.isMsgAvailable()){
          delay(10); //Time to wait to allow the serial module to receive correctly the whole message
          Msg* msg = MsgService.receiveMsg(); 
          const String& validation = msg->getContent();
          if (validation.equals(MSG_VALID)) {
            msgBtService->sendMsg(Msg(MSG_WELCOME));
            token->setState(OPEN_STATE);
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


