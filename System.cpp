#include "System.hpp"
#include "lualib/lua.hpp"

System::System() : devicesAttached(0) {}

bool System::init() {
  return true;
}

Device *System::getDevice(int port) {
  return this->devices[port];
}

bool System::addDevice(int port, Device *device, Setting &sett) {
  if (device->init(sett) == 1) {
    return false;
  }
  this->devices[port] = device;
  this->devicesAttached += 1;
  return true;
}

bool System::removeDevice(int port, bool shouldDelete) {
  if (this->devices[port]->deinit() == 1) {
    return false;
  }
  if (shouldDelete) {
    delete this->devices[port];
  }
  this->devices[port] = NULL;
  this->devicesAttached -= 1;
  return true;
}
