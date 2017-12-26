#ifndef __TOKEN__
#define __TOKEN__

#include "arduino.h"

class Token {
  public:
    Token();
    String getState();
    void setState(String state);
  
  private:
    String state;
};

#endif
