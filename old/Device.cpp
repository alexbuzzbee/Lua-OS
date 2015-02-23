#include "Device.hpp"

const char *Device::type() {
  return this->_type;
}

Device::Device() : _type("abstract") {}
