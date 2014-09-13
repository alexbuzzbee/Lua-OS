#include "lualib/lua.hpp"
#include "hardware.hpp"
#include "System.hpp"
#include "Device.hpp"
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

void hardware_init(lua_State *L) {
  makeLib_start(L);
  makeLib_addFunc(L, l_hardware_getDevice, "getDevice");
  makeLib_addFunc(L, l_hardware_getPortsWithType, "getPortsWithType");
  makeLib_finish(L, "hardware");
}
