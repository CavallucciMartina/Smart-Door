#include <SoftwareServo.h>

#ifndef __ENTERING_TASK__
#define __ENTERING_TASK__

#include "Task.h"
#include "PassiveInfraRed.h"
#include "Arduino.h"
#include <Servo.h>
#include "MsgBtService.h"
#include "Token.h"

class EnteringTask: public Task {

public:
  EnteringTask(Token *token, MsgBtService *msgBtService, Servo servo);
  void init(int period);  
  void tick();

private:
  Token* token;
  Presence* pir;
  Servo servo;
  MsgBtService* msgBtService;
  int doorOpeningTime;
  int arrivalTime;
  enum {OPENING, OPEN} state;
};

#endif
