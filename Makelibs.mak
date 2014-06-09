#Makefile for compiling the 3rd party libraries used
CC=g++
INSTALL_DIR=libheaders
LIBSDIR=3rdparty

all: SOIL luaJIT SDL2

SDL2:
	@echo SDL2

SOIL:
	@echo SOIL

luaJIT:
	@echo luaJIT

clean: