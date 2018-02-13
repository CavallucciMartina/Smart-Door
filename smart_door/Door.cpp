#include "Door.h"

Door::Door(MsgBtService* msgBtService){
    this->msgBtService = msgBtService;
    value = 0;
    distance = false;
    infoToOpen = false;
    openDoor = false;
    closeDoor = false;
    temperature = false;
    led = false;
    permissionOk = false;
    permissionKo = false;
    systemInfo = false;
    username = "";
    password = "";
}

bool Door::isDistance() {
  return distance;
}

bool Door::isInfoToOpen() {
  return infoToOpen;
}
    
bool Door::isOpenDoor() {
  return openDoor;
}

bool Door::isCloseDoor() {
  return closeDoor;
}

bool Door::isTemperature() {
  return temperature;
}

bool Door::isLed() {
  return led;
}

bool Door::isPermissionOk() {
  return permissionOk;
}

bool Door::isPermissionKo(){
  return permissionKo;
}

bool Door::isSystemInfo() {
  return systemInfo;
}

void Door::setValue(int i) {
  value = i;
}

void Door::setDistance(bool b) {
  distance = b;
}

void Door::setInfoToOpen(bool b) {
  infoToOpen = b;
}

void Door::setOpenDoor(bool b) {
  openDoor = b;
}

void Door::setCloseDoor(bool b) {
  closeDoor = b;
}

void Door::setTemperature(bool b) {
  temperature = b;
}

void Door::setLed(bool b) {
  led = b;
}

void Door::setPermissionOk(bool b) {
  permissionOk = b;
}

void Door::setPermissionKo(bool b) {
  permissionKo = b;
}

void Door::setSystemInfo(bool b) {
  systemInfo = b;
}

void Door::setUsername(String s) {
  username = s;
}

void Door::setPassword(String s) {
  password = s;
}

void Door::sendToSerial(String s) {
  MsgService.sendMsg(s);
}

void Door::sendToBt(String s) {
  msgBtService->sendMsg(Msg(s));
}

int Door::getValue() {
  return value;
}

String Door::getUsername() {
  return username;
}

String Door::getPassword() {
  return password;
}

