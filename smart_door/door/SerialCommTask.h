#ifndef __SERIAL_COMM_TASK__
#define __SERIAL_COMM_TASK__

#include "Task.h"
#include "Door.h"

class SerialCommTask: public Task {

  public:

    SerialCommTask(Door* pDoor);
    void init(int period);  
    void tick();
    
  private:
  
    Door* pDoor;
};

#endif
