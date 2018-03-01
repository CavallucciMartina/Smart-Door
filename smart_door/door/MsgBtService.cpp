#include "Arduino.h"
#include "MsgBtService.h"


MsgBtService::MsgBtService(int rxPin, int txPin){
  channel = new SoftwareSerial(rxPin, txPin);
}

void MsgBtService::init(){
  content.reserve(256);
  channel->begin(9600);
}

bool MsgBtService::sendMsg(Msg msg){
  channel->println(msg.getContent());  
}

bool MsgBtService::isMsgAvailable(){
  return channel->available();
}

Msg* MsgBtService::receiveMsg(){
  if (channel->available()){    
    content="";
    while (channel->available()) {
      content += (char)channel->read();      
    }
    return new Msg(content);
  } else {
    return NULL;  
  }
}




