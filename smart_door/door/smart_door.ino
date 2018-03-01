#include "Scheduler.h"
#include "MsgService.h"
#include "MsgBtService.h"
#include "BtCommTask.h"
#include "SerialCommTask.h"
#include "DoorTask.h"
#include "InformationTask.h"
#include "Door.h"
#include "Config.h"

Scheduler sched;
Servo servo;

void setup(){
  Serial.begin(9600);
  while (!Serial){}

  MsgBtService *msg = new MsgBtService(TX_PIN,RX_PIN);
  msg->init();
  Door* pDoor = new Door(msg);
  TempSensor* temp = new TempSensor();
  LedExt* ledValue = new LedExt(LED_VALUE_PIN);

  servo.attach(SERVO_PIN);
  servo.write(0);
  delay(1000);
  servo.detach();
  
  sched.init(100);
  MsgService.init();
  
  BtCommTask* btCommTask = new BtCommTask(pDoor, msg);
  btCommTask->init(100);
  sched.addTask(btCommTask);

  SerialCommTask* serialCommTask = new SerialCommTask(pDoor);
  serialCommTask->init(100);
  sched.addTask(serialCommTask);

  DoorTask* doorTask = new DoorTask(pDoor, ledValue, temp, servo);
  doorTask->init(100);
  sched.addTask(doorTask);

  InformationTask* informationTask = new InformationTask(pDoor, temp, ledValue);
  informationTask->init(100);
  sched.addTask(informationTask);

  delay(1000);
  
}

void loop(){
  sched.schedule();
}
