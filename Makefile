# Makefile for Lua OS.

MAIN_SOURCES = $(wildcard *.cpp)
LUA_SOURCES = $(wildcard lualib/*.c)
MAIN_OBJECTS = ${MAIN_SOURCES:.cpp=.o}
LUA_OBJECTS = ${LUA_SOURCES:lualib/%.c=%.o}

SOURCES = ${MAIN_SOURCES} ${LUA_SOURCES}
OBJECTS = ${MAIN_OBJECTS} ${LUA_OBJECTS}

CC++ = clang++
CC = clang

build: compile
	ld -lc -lc++ -lcrt1.o ${OBJECTS} -o luaos

compile: ${SOURCES}
	${CC++} -c ${MAIN_SOURCES}
	${CC} -c ${LUA_SOURCES}

clean:
	rm *.o
