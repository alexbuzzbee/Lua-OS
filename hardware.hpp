#ifndef hardware_hpp
#define hardware_hpp

int l_hardware_getDevice(lua_State *L);
int l_hardware_getPortsWithType(lua_State *L);
void hardware_doshutdown();
void hardware_create(lua_State *C);
void hardware_init(lua_State *L);

#endif
