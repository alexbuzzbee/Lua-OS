#ifndef util_hpp
#define util_hpp

void makeLib_start(lua_State *L);
void makeLib_finish(lua_State *L, const char *name);
void makeLib_addFunc(lua_State *L, lua_CFunction func, const char *funcName);
void makeLib_addVar(lua_State *L);
void modLib_start(lua_State *L, const char *name);
void modLib_finish(lua_State *L, const char *name);
void modLib_setVar(lua_State *L);
void modLib_getVar(lua_State *L);

#endif
