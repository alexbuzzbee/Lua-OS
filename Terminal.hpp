#ifndef Terminal_hpp
#define Terminal_hpp

#include <curses.h>
#include "Device.hpp"
#include "lualib/lua.hpp"

class Terminal : public Device {
  protected:
    WINDOW *scr;
  public:
    int init(Setting &sett);
    int deinit();
    void getIf(lua_State *L);
    static int di_putstr(lua_State *L);
    static int di_getch(lua_State *L);
    static int di_moveCursor(lua_State *L);
    static int di_setCursorVis(lua_State *L);
    static int di_clear(lua_State *L);
    Terminal();
};

#endif
