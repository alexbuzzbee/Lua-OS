#include "lualib/lua.hpp"
#include "hardware.hpp"
#include "System.hpp"
#include "Device.hpp"
#include "main.hpp"
#include "util.hpp"

int l_hardware_getDevice(lua_State *L) { // Lua proto: hardware.getDevice(port: number): table {port: number, ...}
  int port = luaL_checknumber(L, 1);
  Device *dev = sys->getDevice(port);
  if (dev == NULL) {
    lua_pushstring(L, "empty port");
    lua_error(L);
  }
  dev->getIf(L);
  lua_pushstring(L, "port");
  lua_pushnumber(L, (double)port);
  lua_settable(L, -3);
  return 1;
}

int l_hardware_getPortsWithType(lua_State *L) { // Lua proto: hardware.getPortsWithType(type: string): table {[1: number, ...]}
  const char *type = luaL_checkstring(L, 1);
  lua_newtable(L);
  int table_index = 0;
  for (int i = 0; i < DEVICE_PORTS; i++) {
    Device *dev = sys->getDevice(i);
    if (dev == NULL) {
      break;
    } else if (dev->type() == type) {
      lua_pushnumber(L, table_index++);
      lua_pushnumber(L, i);
      lua_settable(L, -3);
    } else {
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
