#include "lualib/lua.hpp"
#include "main.hpp"
#include "misc.hpp"
#include "util.hpp"

// Usermode protected call. Lua prototype: upcall(function f, number nresults[, number nargs, any arg1, ...]): ...
int l_upcall(lua_State *L) {
  //if (sys.kernelMode) {
    int nresults = luaL_checknumber(L, 2);
    int nargs = luaL_checknumber(L, 3);
    //sys.kernelMode = false;
    lua_pcall(L, nargs, nresults, 0);
    //sys.kernelMode = true;
    return nresults;
  //} else {
    //lua_pushstring("privilege violation");
    //lua_error(L);
  //}
}

void l_misc_init(lua_State *L) {
  //makeLib_start(L);
  //makeLib_addFunc(L, l_misc_loadChunk, "runString");
  //makeLib_finish(L, "misc");
  lua_pushcfunction(L, l_upcall);
  lua_setglobal(L, "upcall");
}
