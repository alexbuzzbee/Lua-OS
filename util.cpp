#include "lualib/lua.hpp"
#include "util.hpp"

void makeLib_start(lua_State *L) {
  lua_newtable(L);
}

void makeLib_finish(lua_State *L, const char *name) {
  lua_setglobal(L, name);
}

// Assumes the library table is at the top.
void makeLib_addFunc(lua_State *L, lua_CFunction func, const char *funcName) {
  lua_pushstring(L, funcName); // Push key.
  lua_pushcfunction(L, func); // Push value.
  lua_settable(L, -3);
}

// Assumes the table is at -3, the value is at -1, and the key is at -2.
void makeLib_addVar(lua_State *L) {
  lua_settable(L, -3);
}

void modLib_start(lua_State *L, const char *name) {
  lua_getglobal(L, name);
}

void modLib_finish(lua_State *L, const char *name) {
  lua_setglobal(L, name);
}

// Assumes the table is at -3, the value is at -1, and the key is at -2.
void modLib_setVar(lua_State *L) {
  lua_settable(L, -3);
}

// Assumes the table is at -2 and the key is at -1.
void modLib_getVar(lua_State *L) {
  lua_gettable(L, -2);
}
