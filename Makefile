# Makefile for Lua OS.

MAIN_SOURCES = $(wildcard *.cpp)
LUA_SOURCES = $(wildcard lualib/*.c)

SOURCES = ${MAIN_SOURCES} ${LUA_SOURCES}

build: ${SOURCES}
	${CC} ${SOURCES}
	mv a.out luaos

clean:
	rm a.out
