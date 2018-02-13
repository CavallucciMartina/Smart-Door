#include "BtCommTask.h"
#include "Config.h"

#define MSG_AUTHENTICATION "A:"
#define MSG_TEMPERATURE "Temp?"
#define MSG_VALUE "Value:"
#define MSG_END_SESSION "End"

BtCommTask::BtCommTask(Door* pDoor, MsgBtService* msgBtService){
  this->pDoor = pDoor;
  this->msgBtService = msgBtService;
}
  
void BtCommTask::init(int period){
  Task::init(period);
}
  
void BtCommTask::tick(){
  if (msgBtService->isMsgAvailable()){
    delay(15); //Time to wait the end of transmission to properly read the string value and the timer
    Msg* msg = msgBtService->receiveMsg(); 
    const String& content = msg->getContent();
    if (content.startsWith(MSG_AUTHENTICATION)){
      content.remove(0,2);
      pDoor->setPassword(content.substring(content.indexOf(":")+1));
      pDoor->setUsername(content.substring(0,content.indexOf(":")));
      pDoor->setInfoToOpen(true);
    } else if (content == MSG_TEMPERATURE){
      pDoor->setTemperature(true);
    } else if (content.startsWith(MSG_VALUE)){
      content.remove(0,6);
      pDoor->setValue(map(content.toInt(),0,100,0,255));
      pDoor->setLed(true);
    } else if (content == MSG_END_SESSION) {
      pDoor->setCloseDoor(true);
    }
    delete msg;    
  }
}

