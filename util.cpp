#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
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

char *util_loadFile(const char *path) {
  char *buff;
  char *part;
  int f;
  int bytesRead;
  off_t size;
  f = open(path, O_RDONLY); // Open file.
  if (f == -1) {
    return NULL;
  }
  size = lseek(f, 0, SEEK_END); // Get file size.
  if (size == -1) {
    close(f);
    return NULL;
  }
  if (lseek(f, 0, SEEK_SET) == -1) {
    close(f);
    return NULL;
  }
  buff = (char *) malloc(size + 1); // Allocate buffers.
  part = (char *) malloc(BLOCKSIZE + 1);
  buff[0] = '\0'; // Initialize first byte to a zero, to make string functions work.
  do {
    bytesRead = read(f, part, BLOCKSIZE);
    if (bytesRead < 0) {
      free(buff);
      free(part);
      close(f);
      return NULL;
    }
    part[bytesRead] = '\0'; // Add a zero to the end.
    strcpy(buff, part); // Append part to buffer.
  } while (bytesRead != 0);
  free(part);
  close(f);
  return buff;
}

bool util_dumpFile(const char *path, char *buff) {
  int f;
  int bytesWritten = 0;
  int bytesToWrite;
  if (truncate(path, 0) == -1) { // Truncate file to 0 length for simplicity.
    return false;
  }
  f = open(path, O_WRONLY | O_CREAT, 0644); // Open the file.
  if (f == -1) {
    return false;
  }
  bytesToWrite = strlen(buff); // Set up bytesToWrite.
  do {
    int justWritten = write(f, buff, bytesToWrite - bytesWritten); // Write remaining bytes.
    if (justWritten == -1) {
      close(f);
      return false;
    }
    bytesWritten += justWritten; // Increase bytesWritten by the amount just written.
  } while (bytesWritten < bytesToWrite);
  close(f);
  return true;
}
