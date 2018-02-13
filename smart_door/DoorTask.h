#ifndef __DOOR_TASK__
#define __DOOR_TASK__

#include "Task.h"
#include "DoorTask.h"
#include "Sonar.h"
#include "PassiveInfraRed.h"
#include "Config.h"
#include "LedExt.h"
#include "Led.h"
#include "ButtonImpl.h"
#include "TempSensor.h"
#include "Door.h"
#include <Servo.h>

class DoorTask: public Task {

public:
  DoorTask(Door* pDoor, LedExt* ledExt, TempSensor* temp, Servo servo);
  void init(int period);  
  void tick();

private:
  Servo servo;
  Door* pDoor;
  ProximitySensor* prox;
  Led* ledOn;
  LedExt* ledValue;
  TempSensor* temp;
  PassiveInfraRed* pir;
  ButtonImpl* buttonEnd;
  int timeToWait;
  int arrivalTime;
  enum {IDLE, WAITING, AUTHENTICATION, OPENING, OPEN, CLOSING, SESSION} state;
};

#endif
