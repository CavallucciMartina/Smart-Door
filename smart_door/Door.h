#ifndef __DOOR__
#define __DOOR__

#include "MsgService.h"
#include "MsgBtService.h"

class Door {

  public:
    Door(MsgBtService* msgBtService);
    
    bool isDistance();
    bool isInfoToOpen();
    bool isOpenDoor();
    bool isCloseDoor();
    bool isTemperature();
    bool isLed();
    bool isPermissionOk();
    bool isPermissionKo();
    bool isSystemInfo();

    void setValue(int i);
    void setDistance(bool b);
    void setInfoToOpen(bool b);
    void setOpenDoor(bool b);
    void setCloseDoor(bool b);
    void setTemperature(bool b);
    void setLed(bool b);
    void setPermissionOk(bool b);
    void setPermissionKo(bool b);
    void setSystemInfo(bool b);
    void setUsername(String s);
    void setPassword(String s);

    void sendToSerial(String s);
    void sendToBt(String s);

    int getValue();
    String getUsername();
    String getPassword(); 

  private:
    MsgBtService* msgBtService;
    int value;
    bool distance;
    bool infoToOpen;
    bool openDoor;
    bool closeDoor;
    bool temperature;
    bool led;
    bool permissionOk;
    bool permissionKo;
    bool systemInfo;
    String username;
    String password;
};

#endif
