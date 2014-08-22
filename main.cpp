#include <stdio.h>
#include <string.h>
#include "lualib/lua.hpp"
#include "util.hpp"

// System sys = System() // The system.

// Lua prototype: utils.runString(code, name, nresults)
int l_utils_runString(lua_State *L) {
  const char *code = luaL_checkstring(L, 1);
  const char *name = luaL_checkstring(L, 2);
  int nresults = luaL_checknumber(L, 3);
  int error = luaL_loadbuffer(L, code, strlen(code), name) ||
    lua_pcall(L, 0, nresults, 0);
  if (error) {
    lua_pushboolean(L, false);
    return 1;
  }
  else {
    return nresults;
  }
}

// Usermode protected call. Lua prototype: upcall(f, nresults[, nargs, arg1, ...])
int l_upcall(lua_State *L) {
  int nresults = luaL_checknumber(L, 2);
  int nargs = luaL_checknumber(L, 3);
  //sys.kernelMode = false;
  lua_pcall(L, nargs, nresults, 0);
  //sys.kernelMode = true;
  return nresults;
}

void l_utils_init(lua_State *L) {
  makeLib_start(L);
  makeLib_addFunc(L, l_utils_runString, "runString");
  makeLib_finish(L, "utils");
  lua_pushcfunction(L, l_upcall);
  lua_setglobal(L, "upcall");
}

int main() {
  lua_State *L = luaL_newstate();
  printf("Lua OS; Lua version: %s R%s (modified).\n", LUA_VERSION, LUA_VERSION_RELEASE);
  luaL_openlibs(L);
  l_utils_init(L);
  lua_close(L);
  return 0;
}
