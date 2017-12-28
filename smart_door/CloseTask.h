#ifndef __CLOSE_TASK__
#define __CLOSE_TASK__

#include "Task.h"
#include "Arduino.h"
#include <Servo.h>
#include "Token.h"

class CloseTask: public Task {

public:
  CloseTask(Token *token, Servo servo);
  void init(int period);  
  void tick();

private:
  Token* token;
  Servo servo;
  int doorClosingTime;
  enum {CLOSING} state;
};

#endif
