#include "System.hpp"
#include "lualib/lua.hpp"

System::System() : devicesAttached(0) {}

void *System::getDevice(int port) {
  return this->devices[port];
}

bool System::addDevice(int port, void *device) {
  this->devices[port] = device;
  return true;
}

bool System::removeDevice(int port, bool shouldDelete) {
  if (shouldDelete) {
    delete this->devices[port];
  }
  this->devices[port] = NULL;
  this->devicesAttached -= 1;
}
