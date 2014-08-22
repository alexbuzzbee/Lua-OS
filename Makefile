# Makefile for Lua OS.

MAIN_SOURCES = $(wildcard *.cpp)
LUA_SOURCES = lualib/lapi.c lualib/lauxlib.c lualib/lbaselib.c lualib/lbitlib.c lualib/lcode.c lualib/lcorolib.c lualib/lctype.c lualib/ldblib.c lualib/ldebug.c lualib/ldo.c lualib/ldump.c lualib/lfunc.c lualib/lgc.c lualib/linit.c lualib/liolib.c lualib/llex.c lualib/lmathlib.c lualib/lmem.c lualib/loadlib.c lualib/lobject.c lualib/lopcodes.c lualib/loslib.c lualib/lparser.c lualib/lstate.c lualib/lstring.c lualib/lstrlib.c lualib/ltable.c lualib/ltablib.c lualib/ltm.c lualib/lundump.c lualib/lvm.c lualib/lzio.c

SOURCES = ${MAIN_SOURCES} ${LUA_SOURCES}

build: ${SOURCES}
	${CC} ${SOURCES}

clean:
	rm a.out
