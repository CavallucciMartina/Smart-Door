#ifndef __BT_COMM_TASK__
#define __BT_COMM_TASK__

#include "Task.h"
#include "Door.h"

class BtCommTask: public Task{

  public:

    BtCommTask(Door* pDoor, MsgBtService* msgBtService);
    void init(int period);  
    void tick();
    
  private:
  
    Door* pDoor;
    MsgBtService* msgBtService;
};

#endif
