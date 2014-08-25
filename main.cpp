#include <stdio.h>
#include <string.h>
#include "lualib/lua.hpp"
#include "util.hpp"
#include "misc.hpp"
//#include "main.hpp"

void state_init(lua_State *L) {
  luaL_openlibs(L);
  l_misc_init(L);
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

int main() {
  lua_State *L = luaL_newstate();
  printf("Lua OS; Lua version: %s R%s (modified).\n", LUA_VERSION, LUA_VERSION_RELEASE);
  state_init(L);
  lua_close(L);
  return 0;
}
