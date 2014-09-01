# Lua OS makefile for Mac OS X.

MAIN_SOURCES = $(wildcard *.cpp)
LUA_SOURCES = $(wildcard lualib/*.c)
MAIN_OBJECTS = ${MAIN_SOURCES:.cpp=.o}
LUA_OBJECTS = ${LUA_SOURCES:lualib/%.c=%.o}

SOURCES = ${MAIN_SOURCES} ${LUA_SOURCES}
OBJECTS = ${MAIN_OBJECTS} ${LUA_OBJECTS}

CC++ = clang++
CC = clang
LD = ld

LFLAGS = -lc -lc++ -lcrt1.o -lncurses -o luaos -macosx_version_min 10.8
CFLAGS = -c

build: compile
	${LD} ${LFLAGS} ${OBJECTS}

compile: ${SOURCES}
	${CC++} ${CFLAGS} ${MAIN_SOURCES}
	${CC} ${CFLAGS} ${LUA_SOURCES}

clean:
	rm *.o
