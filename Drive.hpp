#ifndef Drive_hpp
#define Drive_hpp

#include "Device.hpp"

class Drive : public Device {
  public:
    const char *name;
    int init();
    int deinit();
    void getIf(lua_State *L);
    static int di_load(lua_State *L);
    static int di_save(lua_State *L);
    static int di_delete(lua_State *L);
    static int di_eraseDrive(lua_State *L);
    Drive(const char *name);
};

#endif
