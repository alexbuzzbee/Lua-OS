#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include "lualib/lua.hpp"
#include "Drive.hpp"
#include "main.hpp"
#include "util.hpp"

char *resolveFilen(const char *drvName, int filen) {
  char *str_filen = (char *) malloc(5);
  char *fpath = (char *) malloc(257);
  getcwd(fpath, 257); // Set fpath to the current directory.
  strcat(fpath, "/drives/"); // Update fpath to the correct path.
  strcat(fpath, drvName); strcat(fpath, "/");
  sprintf(str_filen, "%d", filen);
  strcat(fpath, (const char *) str_filen);
  free(str_filen);
  return fpath;
}

Drive::Drive() {
  _type = "drive";
}

void Drive::getIf(lua_State *L) {
  makeLib_start(L);
  makeLib_addFunc(L, Drive::di_load, "load");
  makeLib_addFunc(L, Drive::di_save, "save");
  makeLib_addFunc(L, Drive::di_delete, "delete");
  makeLib_addFunc(L, Drive::di_eraseDrive, "eraseDrive");
}

int Drive::init(libconfig::Setting &sett) {
  this->name = sett["name"];
  return 0;
}

int Drive::deinit() {
  return 0;
}

int Drive::di_load(lua_State *L) { // Lua proto: drive:load(filen: number): string OR nil
  int filen;
  int port;
  char *fpath;
  Drive *self;
  char *fdata;
  filen = (int) luaL_checknumber(L, 2); // Fetch file number argument.
  lua_pushstring(L, "port"); // Fetch self.
  lua_gettable(L, 1);
  port = (int) lua_tonumber(L, -1);
  self = (Drive *) sys->getDevice(port);
  fpath = resolveFilen(self->name, filen); // Get path.
  fdata = util_loadFile(fpath);
  free(fpath);
  if (fdata == NULL) {
    free(fdata);
    return 0;
  }
  lua_pushstring(L, fdata);
  free(fdata);
  return 1;
}

int Drive::di_save(lua_State *L) { // Lua proto: drive:save(filen: number, data: string): boolean
  int filen;
  int port;
  char *fpath;
  Drive *self;
  char *data;
  filen = (int) luaL_checknumber(L, 2); // Fetch file number argument.
  data = (char *) luaL_checkstring(L, 3); // Fetch data argument.
  lua_pushstring(L, "port"); // Fetch self.
  lua_gettable(L, 1);
  port = (int) lua_tonumber(L, -1);
  self = (Drive *) sys->getDevice(port);
  fpath = resolveFilen(self->name, filen); // Get path.
  lua_pushboolean(L, util_dumpFile(fpath, data)); // Return the result of dumpFile.
  return 1;
}

int Drive::di_delete(lua_State *L) { // Lua proto: drive:delete(filen: number): boolean
  int filen;
  int port;
  char *fpath;
  Drive *self;
  filen = (int) luaL_checknumber(L, 2); // Fetch file number argument.
  lua_pushstring(L, "port"); // Fetch self.
  lua_gettable(L, 1);
  port = (int) lua_tonumber(L, -1);
  self = (Drive *) sys->getDevice(port);
  fpath = resolveFilen(self->name, filen); // Get path.
  if (unlink(fpath) == -1) { // Try to unlink the file.
    lua_pushboolean(L, false);
    return 1;
  }
  lua_pushboolean(L, true);
  return 1;
}

int Drive::di_eraseDrive(lua_State *L) { // Lua proto: drive:eraseDrive(): boolean
  DIR *dp;
  struct dirent *ep;
  int port;
  Drive *self;
  char *dpath = (char *) malloc(257);
  lua_pushstring(L, "port"); // Fetch self.
  lua_gettable(L, 1);
  port = (int) lua_tonumber(L, -1);
  self = (Drive *) sys->getDevice(port);
  getcwd(dpath, 257); // Set dpath to the current directory.
  strcat(dpath, "/drives/"); // Update dpath to the correct path.
  strcat(dpath, self->name);
  dp = opendir(dpath);
  if (dp != NULL) {
    char *delpath = (char *) malloc(257); // Path to file to delete.
    while ((ep = readdir(dp)) != NULL) {
      getcwd(delpath, 257); // Set delpath to the current directory.
      strcat(delpath, "/drives/"); // Update delpath to the correct path.
      strcat(delpath, self->name); strcat(delpath, "/");
      strcat(delpath, ep->d_name);
      unlink(delpath);
    }
    free(delpath);
    closedir (dp);
  } else {
    free(dpath);
    lua_pushboolean(L, false);
    return 1;
  }
  free(dpath);
  lua_pushboolean(L, true);
  return 1;
}
