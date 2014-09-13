#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "lualib/lua.hpp"
#include "util.hpp"
#include "main.hpp"
#include "hardware.hpp"
#include "System.hpp"
#include "Terminal.hpp"

System *sys;

void state_init(lua_State *L) {
  luaL_openlibs(L);
  // Sandboxing: remove loadfile(), print(), dofile(), and collectgarbage().
  lua_pushnil(L);
  lua_setglobal(L, "loadfile");
  lua_pushnil(L);
  lua_setglobal(L, "print");
  lua_pushnil(L);
  lua_setglobal(L, "dofile");
  lua_pushnil(L);
  lua_setglobal(L, "collectgarbage");
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    chdir(argv[1]);
  }
  lua_State *L = luaL_newstate();
  lua_State *C = luaL_newstate(); // Configuration lua state.
  state_init(L);
  sys = new System;
  hardware_create(C);
  lua_close(C);
  printf("Lua OS; Lua version: %s R%s (modified).\n", LUA_VERSION, LUA_VERSION_RELEASE);
  lua_close(L);
  return 0;
}
