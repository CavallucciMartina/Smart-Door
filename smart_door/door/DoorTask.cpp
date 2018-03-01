#include "DoorTask.h"

#define MSG_START_WORKING "S:B"
#define MSG_END_SESSION "S:S"
#define MSG_TIMEOUT "S:T"
#define MSG_TEMP "Temp:"
#define MSG_HELLO "Hello"
#define MSG_TOO_FAR "Bye"
#define MSG_SUCCESS "Valid:T"
#define MSG_FAIL "Valid:F"
#define DELTA 0.4

DoorTask::DoorTask(Door* pDoor, LedExt* ledExt, TempSensor* temp, Servo servo){
  this->servo = servo;
  this->pDoor = pDoor;
  this->temp = temp;
  pir = new PassiveInfraRed(PIR_PIN);
  prox = new Sonar(PROX_ECHO_PIN,PROX_TRIG_PIN);
  ledOn = new Led(LED_CONNECTED_PIN);
  buttonEnd = new ButtonImpl(BUTTON_PIN);
  this->ledValue = ledExt;
}
  
void DoorTask::init(int period){
  Task::init(period);
  state = IDLE;
  ledOn->switchOn();
  ledValue->switchOn();
  arrivalTime = 0;
  timeToWait = 0;
}
  
void DoorTask::tick(){
    switch (state){
    
      case IDLE: {
        if (prox->getDistance() <= MIN_DIST) {
          pDoor->setPermissionOk(false);
          state = WAITING;
        }
        break;
      }

      case WAITING: {
        timeToWait += myPeriod;
        if (prox->getDistance() > MIN_DIST + DELTA) {
          state = IDLE;
        } else if (timeToWait >= MIN_SEC*1000) {
          state = AUTHENTICATION;
          timeToWait = 0;
          pDoor->sendToBt(MSG_HELLO);
        }
        break;
      }

      case AUTHENTICATION: {
        if (prox->getDistance() > MIN_DIST + DELTA) {
          state = IDLE;
          pDoor->sendToBt(MSG_TOO_FAR);
        } else if (pDoor->isInfoToOpen()){
          pDoor->sendToSerial("A:" + pDoor->getUsername() + ":" + pDoor->getPassword());
          pDoor->setInfoToOpen(false);
        }else if (pDoor->isPermissionOk()){
          pDoor->sendToBt(MSG_SUCCESS);
          pDoor->setPermissionOk(false);
          state = OPENING;
          pDoor->setOpenDoor(false);
        }else if (pDoor->isPermissionKo()){
          pDoor->sendToBt(MSG_FAIL);
          state = IDLE;
          pDoor->setPermissionKo(false);
        }
        break;
      }
      
      case OPENING: {
        servo.attach(SERVO_PIN);
        servo.write(OPEN_DEG);
        delay(500); // Time to wait the servo to move from 0 to 175 degrees
        servo.detach();
        state = OPEN;
        break;
      }

      case OPEN: {
        arrivalTime += myPeriod;
        if (arrivalTime < MAX_DELAY && pir->isPresent()){
          arrivalTime = 0;
          state = SESSION;
          pDoor->sendToSerial(MSG_START_WORKING);
          pDoor->sendToBt(MSG_START_WORKING);
        } else if (arrivalTime >= MAX_DELAY){
          arrivalTime = 0;
          state = CLOSING;
          pDoor->setCloseDoor(true);
          pDoor->setOpenDoor(false);
          pDoor->sendToSerial(MSG_TIMEOUT);
          pDoor->sendToBt(MSG_TIMEOUT);
        }
        break;
      }

      case CLOSING: {
        if (pDoor->isCloseDoor()) {
          servo.attach(SERVO_PIN);
          servo.write(CLOSE_DEG); 
          delay(500); //Time to wait the servo to move from 175 to 0 degree
          servo.detach();
          pDoor->setCloseDoor(false);
          state = IDLE;    
        }
        break;
      }

      case SESSION: {
        if (pDoor->isCloseDoor() || buttonEnd->isPressed()) {
          pDoor->setCloseDoor(true);
          pDoor->sendToSerial(MSG_END_SESSION);
          pDoor->sendToBt(MSG_END_SESSION);
          state = CLOSING;
        } else if (pDoor->isTemperature()) {
          pDoor->sendToBt(MSG_TEMP + String(temp->readTemperature()));
          pDoor->setTemperature(false);
        } else if (pDoor->isLed()){
          ledValue->setIntensity(pDoor->getValue());
          pDoor->setLed(false);
        }
      }
      break;
   } 
}


