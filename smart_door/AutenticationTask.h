#ifndef __IDENTICATION_TASK__
#define __IDENTICATION_TASK__

#include "Task.h"
#include "ProximitySensor.h"
#include "MsgBtService.h"
#include "Led.h"
#include "Token.h"

class AutenticationTask: public Task {

public:
  AutenticationTask(Token *token, MsgBtService *msgBtService);
  void init(int period);  
  void tick();

private:
  Token* token;
  ProximitySensor* prox;
  Led* ledOn;
  MsgBtService* msgBtService;
  enum { IDLE, USR, PWD } state;
};

#endif
