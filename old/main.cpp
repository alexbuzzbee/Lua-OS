#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include "lualib/lua.hpp"
#include "util.hpp"
#include "main.hpp"
#include "hardware.hpp"
#include "System.hpp"

System *sys;

void state_init(lua_State *L) {
  luaL_openlibs(L);
  hardware_init(L);
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
  if (L == NULL) {
    printf("Error: Allocating main lua state failed!\n");
    exit(1);
  }
  char *firm;
  size_t firmlen;
  sys = new System;
  printf("Loading firmware...\n");
  refresh();
  state_init(L); // Initialize L.
  firm = util_loadFile("firmware.lua"); // Load firmware.
  if (firm == NULL) { // Handle load errors.
    printf("Error: failed to load file firmware.lua\n");
    refresh();
    hardware_doshutdown();
    lua_close(L);
    exit(1);
  }
  firmlen = strlen(firm);
  if (luaL_loadbuffer(L, firm, firmlen, "firmware") != 0) {
    printf("Error: failed to parse system firmware.\n");
    refresh();
    hardware_doshutdown();
    lua_close(L);
    free(firm);
    exit(1);
  }
  free(firm);
  printf("Lua OS; Lua version: %s R%s (modified).\n\r", LUA_VERSION, LUA_VERSION_RELEASE); // Display heading.
  refresh();
  hardware_create();
  lua_pcall(L, 0, 0, 0); // Run firmware.
  lua_close(L);
  return 0;
}
