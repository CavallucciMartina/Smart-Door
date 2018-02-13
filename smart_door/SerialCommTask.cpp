#include "SerialCommTask.h"
#include "Config.h"

#define MSG_VALID_OK "Valid:T"
#define MSG_VALID_KO "Valid:F"
#define MSG_INFORMATION "InfoRequest"

SerialCommTask::SerialCommTask(Door* pDoor){
  this->pDoor = pDoor;
}
  
void SerialCommTask::init(int period){
  Task::init(period);
}
  
void SerialCommTask::tick(){
  if (MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg(); 
    const String& content = msg->getContent();
    if (content == MSG_VALID_OK){
      pDoor->setPermissionOk(true);
    } else if (content == MSG_VALID_KO){
      pDoor->setPermissionKo(true);
    } else if (content == MSG_INFORMATION){
      pDoor->setSystemInfo(true);
    }
    delete msg;    
  }
}

