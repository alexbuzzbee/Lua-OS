#include <stdlib.h>
#include <string.h>
#include <libconfig.h++>
#include "lualib/lua.hpp"
#include "hardware.hpp"
#include "System.hpp"
#include "Device.hpp"
#include "Drive.hpp"
#include "Terminal.hpp"
#include "main.hpp"
#include "util.hpp"

int l_hardware_getDevice(lua_State *L) { // Lua proto: hardware.getDevice(port: number): table {port: number, ...}
  int port = (int) luaL_checknumber(L, 1); //  Fetch the port.
  Device *dev = sys->getDevice(port); // Grab the device.
  if (dev == NULL) { // Means no such device.
    lua_pushstring(L, "empty port"); // Throw "empty port" error.
    lua_error(L);
    return 0;
  }
  dev->getIf(L); // Get the device interface.
  lua_pushstring(L, "port"); // Set the "port" field to the port of the device.
  lua_pushnumber(L, (double)port);
  lua_settable(L, -3);
  lua_pushstring(L, "type"); // Set the "type" field to the type of the device.
  lua_pushstring(L, dev->type());
  lua_settable(L, -3);
  return 1;
}

int l_hardware_getPortsWithType(lua_State *L) { // Lua proto: hardware.getPortsWithType(type: string): table {[1: number, ...]}
  const char *type = luaL_checkstring(L, 1); // Fetch the requested type.
  lua_newtable(L); // Create a table to hold the return data.
  int table_index = 0; // Current index of the table.
  for (int i = 0; i < DEVICE_PORTS; i++) { // Loop through all devices attached to the system.
    Device *dev = sys->getDevice(i); // Grab the device.
    if (dev == NULL) { // Means no such device.
      break; // Break the loop and return the table.
    } else if (dev->type() == type) {
      lua_pushnumber(L, ++table_index); // Add the port to the table.
      lua_pushnumber(L, i);
      lua_settable(L, -3);
    } else { // Next iteration.
      continue;
    }
  }
  return 1;
}

int l_hardware_shutdown(lua_State *L) { // Lua proto: hardware.shutdown(): nil
  hardware_doshutdown();
  return 0;
}

int l_hardware_gettime(lua_State *L) {
  lua_pushnumber(L, time(null));
  return 1;
}

void hardware_doshutdown() {
  for (int i = 0; i < DEVICE_PORTS; i++) { // Loop through all devices attached to the system.
    Device *dev = sys->getDevice(i); // Grab the device.
    if (dev == NULL) { // Means no such device.
      break; // Break the loop.
    } else { // Next iteration.
      sys->removeDevice(i, true);
    }
  }
  exit(0); // Exit the Lua OS application.
}

void hardware_create() { // Creates the hardware representation.
  sys->init();
  libconfig::Config *cfg = new libconfig::Config();
  cfg->readFile("luaos.cfg");
  libconfig::Setting &sett = cfg->lookup("devices");
  size_t lsett = sett.getLength();
  for (int i = 0; i < lsett; i++) {
    const char *type = sett[i]["type"];
    if (strcmp(type, "drive") == 0) {
      sys->addDevice(sett[i]["port"], new Drive(), sett[i]["conf"]);
    } else if (strcmp(type, "terminal") == 0) {
      sys->addDevice(sett[i]["port"], new Terminal(), sett[i]["conf"]);
    } else {
      printf("WARNING: bad device type '%s'\n", type);
    }
  }
}

void hardware_init(lua_State *L) {
  makeLib_start(L);
  makeLib_addFunc(L, l_hardware_getDevice, "getDevice");
  makeLib_addFunc(L, l_hardware_getPortsWithType, "getPortsWithType");
  makeLib_addFunc(L, l_hardware_shutdown, "shutdown");
  makeLib_finish(L, "hardware");
}
