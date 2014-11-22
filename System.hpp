#ifndef System_hpp
#define System_hpp

#include "Device.hpp"
#include <libconfig.h++>

#define DEVICE_PORTS 10

class System {
  private:
    Device *devices[DEVICE_PORTS]; // Devices attached to the system.
    int devicesAttached;
  public:
    bool init();
    Device *getDevice(int port);
    bool addDevice(int port, Device *device, Setting &sett);
    bool removeDevice(int port, bool shouldDelete);
    System();
};

#endif
