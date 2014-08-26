#ifndef System_hpp
#define System_hpp

#define DEVICE_PORTS 10

class System {
  private:
    void *devices[DEVICE_PORTS]; // Devices attached to the system.
    int devicesAttached;
  public:
    void *getDevice(int port);
    bool addDevice(int port, void *device);
    bool removeDevice(int port);
    System();
};

#endif
