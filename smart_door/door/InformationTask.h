#ifndef __INFORMATION_TASK__
#define __INFORMATION_TASK__

#include "Task.h"
#include "Door.h"
#include "TempSensor.h"
#include "LedExt.h"

class InformationTask: public Task {

  public:

    InformationTask(Door* pDoor, TempSensor* temp, LedExt* ledValue);
    void init(int period);  
    void tick();
    
  private:
  
    Door* pDoor;
    TempSensor* temp;
    LedExt* ledValue;
};

#endif
