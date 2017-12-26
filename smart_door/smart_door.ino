#include "Scheduler.h"
#include "MsgService.h"
#include "MsgBtService.h"
#include "AutenticationTask.h"
#include "Token.h"

Scheduler sched;


void setup(){
  Serial.begin(9600);
  while (!Serial){}
  
  MsgBtService *msg = new MsgBtService(2,3);
  msg->init();
  Token *token = new Token();
  
  sched.init(100);
  MsgService.init();
  
  AutenticationTask* autenticationTask = new AutenticationTask(token, msg);
  autenticationTask->init(100);
  sched.addTask(autenticationTask);
}

void loop(){
  sched.schedule();
}
