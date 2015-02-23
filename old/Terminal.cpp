#include <curses.h>
#include "Terminal.hpp"
#include "util.hpp"
#include "main.hpp"

Terminal::Terminal() {
  _type = "terminal";
}

int Terminal::init(libconfig::Setting &sett) {
  this->scr = initscr();
  cbreak();
  noecho();
  keypad(this->scr, TRUE);
  nodelay(this->scr, FALSE);
  erase();
  refresh();
  return 0;
}

int Terminal::deinit() {
  endwin();
  return 0;
}

void Terminal::getIf(lua_State *L) {
  makeLib_start(L);
  makeLib_addFunc(L, Terminal::di_putstr, "putstr");
  makeLib_addFunc(L, Terminal::di_getch, "getstr");
  makeLib_addFunc(L, Terminal::di_moveCursor, "moveCursor");
  makeLib_addFunc(L, Terminal::di_setCursorVis, "setCursorVis");
  makeLib_addFunc(L, Terminal::di_clear, "clear");
}

int Terminal::di_putstr(lua_State *L) { // Lua proto: term:putstr(str: string): nil
  const char *str = luaL_checkstring(L, 2);
  printw("%s", str);
  refresh();
  return 0;
}

int Terminal::di_getch(lua_State *L) { // Lua proto: term:getch(): string OR number OR nil | WARNING: freezes system!
  int c = getch(); // Grab character.
  if (c == 27 /* ESC */ || c == KEY_BACKSPACE || c == KEY_DC || c == KEY_UP || c == KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT || c == KEY_ENTER) { // Key is recognized special character.
    lua_pushnumber(L, (double)c); // Return the key number.
    return 1;
  } else if (c < KEY_MIN) { // Key is ASCII.
    char str[2]; // Return the character as a length-1 string.
    str[0] = (char)c;
    str[1] = '\0';
    lua_pushstring(L, str);
    return 1;
  } else { // Key is unrecognized.
    return 0;
  }
}

int Terminal::di_moveCursor(lua_State *L) { // Lua proto: term:moveCursor(x: number, y: number): number
  int x = (int) luaL_checknumber(L, 2);
  int y = (int) luaL_checknumber(L, 3);
  lua_pushnumber(L, (double)move(y, x));
  refresh();
  return 1;
}

int Terminal::di_setCursorVis(lua_State *L) { // Lua proto: term:setCursorVis(visible: boolean): nil
  bool visible = lua_toboolean(L, 2);
  if (visible) {
    curs_set(1);
  } else {
    curs_set(0);
  }
  refresh();
  return 0;
}

int Terminal::di_clear(lua_State *L) { // Lua proto: term:clear(): nil
  erase();
  refresh();
  return 0;
}
