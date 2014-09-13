#ifndef Device_hpp
#define Device_hpp

#include "lualib/lua.hpp"

class Device {
  protected:
    const char *_type;
  public:
    virtual int init(lua_State *C) =0;
    virtual int deinit() =0;
    const char *type();
    virtual void getIf(lua_State *L) =0;
    Device();
};

#endif
