#include "WorkingTask.h"
#include "Led.h"
#include "Button.h"
#include "MsgService.h"
#include "TempSensor.h"
#include "Config.h"

#define MSG_END "END"
#define MSG_ASK_TEMP "TEMP?"
#define MSG_SEND_TEMP "TEMP: "
#define MSG_SET_VALUE "SET: "
#define MSG_ASK_VALUE "VALUE?"
#define MSG_LED_VALUE "LED VALUE: "

WorkingTask::WorkingTask(Token *token, MsgBtService *msgBtService){
  this->token = token;
  this->msgBtService = msgBtService;
  temp = new TempSensor();
  ledValue = new Led(LED_VALUE_PIN);
  buttonEnd = new ButtonImpl(BUTTON_PIN);
}
  
void WorkingTask::init(int period){
  Task::init(period);
  state = WORKING;
}
  
void WorkingTask::tick(){
  if (token->getState() == WORKING_STATE) {
    switch (state){
    
      case WORKING: {
        if (msgBtService->isMsgAvailable()){
          delay(10); //Time to wait to allow the bluetooth module to receive correctly the whole message
          Msg* msg = msgBtService->receiveMsg();
          const String& message = msg->getContent();
          if (message == MSG_ASK_TEMP) {
            int value = temp->readTemperature();
            msgBtService->sendMsg(Msg(MSG_SEND_TEMP + String(value)));
          } else if (message.startsWith(MSG_SET_VALUE)) {
            String value = message; 
            value.remove(0, 5);
            if (value == "ON") {
              ledValue->switchOn();
            } else if (value == "OFF"){
              ledValue->switchOff();
            }
          } else if (message == MSG_END){
            token->setState(CLOSE_STATE);
            msgBtService->sendMsg(String(MSG_END));
            MsgService.sendMsg(MSG_END);
            break;
          }
        } else if (MsgService.isMsgAvailable()) {
          delay(10); //Time to wait to allow the serial to receive correctly the whole message
          Msg* msg = MsgService.receiveMsg(); 
          const String& data = msg->getContent();
          if (data == MSG_ASK_TEMP) {
            int value = temp->readTemperature();
            MsgService.sendMsg(MSG_SEND_TEMP + String(value));
          } else if (data == MSG_ASK_VALUE) {
            String ledStatus;
            ledValue->getStatus() ? ledStatus = "ON" : ledStatus = "OFF";
            MsgService.sendMsg(MSG_SEND_TEMP + ledStatus);
          }
        } else if (buttonEnd->isPressed()){
          token->setState(CLOSE_STATE);
          msgBtService->sendMsg(String(MSG_END));
          MsgService.sendMsg(MSG_END);
          break;
        }
      }
    }
  }
}


