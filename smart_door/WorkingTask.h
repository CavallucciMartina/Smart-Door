#ifndef __WORKING_TASK__
#define __WORKING_TASK__

#include "Task.h"
#include "TempSensor.h"
#include "ButtonImpl.h"
#include "MsgBtService.h"
#include "Led.h"
#include "Token.h"

class WorkingTask: public Task {

public:
  WorkingTask(Token *token, MsgBtService *msgBtService);
  void init(int period);  
  void tick();

private:
  Token* token;
  TempSensor* temp;
  Led* ledValue;
  Button* buttonEnd;
  MsgBtService* msgBtService;
  enum {WORKING} state;
};

#endif
