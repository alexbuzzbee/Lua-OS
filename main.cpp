#include <stdio.h>
#include <string.h>
#include "lualib/lua.hpp"
#include "util.hpp"
#include "misc.hpp"
//#include "main.hpp"

int main() {
  lua_State *L = luaL_newstate();
  printf("Lua OS; Lua version: %s R%s (modified).\n", LUA_VERSION, LUA_VERSION_RELEASE);
  luaL_openlibs(L);
  l_misc_init(L);
  lua_close(L);
  return 0;
}
