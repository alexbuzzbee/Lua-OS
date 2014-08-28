#include "System.hpp"
#include "lualib/lua.hpp"

System::System() : devicesAttached(0) {}

Device *System::getDevice(int port) {
  return this->devices[port];
}

bool System::addDevice(int port, Device *device) {
  this->devices[port] = device;
  this->devicesAttached += 1;
  return true;
}

bool System::removeDevice(int port, bool shouldDelete) {
  if (shouldDelete) {
    delete this->devices[port];
  }
  this->devices[port] = NULL;
  this->devicesAttached -= 1;
  return true;
}
