#!/bin/sh
if [ ! -d ../out/dbg/ ]; then
	mkdir -p ../out/dbg/
fi
if [ ! -d ../out/rel/ ]; then
	mkdir -p ../out/rel/
fi
if [ -f ../out/dbg/cg ]; then
	rm ../out/dbg/cg
fi
if [ -f ../out/rel/cg ]; then
	rm ../out/rel/cg
fi

clang \
	-Wall \
	-O0 \
	-o ../out/dbg/cg \
	../src/screen.c \
	../src/input.c \
	../src/cg.c \

clang \
	-Wall \
	-O2 \
	-o ../out/rel/cg \
	../src/screen.c \
	../src/input.c \
	../src/cg.c \
