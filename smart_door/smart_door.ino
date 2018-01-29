#include "Scheduler.h"
#include "MsgService.h"
#include "MsgBtService.h"
#include "AutenticationTask.h"
#include "EnteringTask.h"
#include "CloseTask.h"
#include "WorkingTask.h"
#include "Token.h"
#include "Config.h"

Scheduler sched;
Servo servo;

void setup(){
  Serial.begin(9600);
  while (!Serial){}

  MsgBtService *msg = new MsgBtService(TX_PIN,RX_PIN);
  msg->init();
  Token *token = new Token();

  servo.attach(SERVO_PIN);
  servo.write(0);
  delay(1000);
  servo.detach();
  
  sched.init(100);
  MsgService.init();
  
  AutenticationTask* autenticationTask = new AutenticationTask(token, msg);
  autenticationTask->init(100);
  sched.addTask(autenticationTask);

  EnteringTask* enteringTask = new EnteringTask(token, msg, servo);
  enteringTask->init(100);
  sched.addTask(enteringTask);

  CloseTask* closeTask = new CloseTask(token, servo);
  closeTask->init(100);
  sched.addTask(closeTask);

  WorkingTask* workingTask = new WorkingTask(token, msg);
  workingTask->init(100);
  sched.addTask(workingTask);
  
}

void loop(){
  sched.schedule();
}
