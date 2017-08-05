#!/bin/sh
if [ ! -d ../out/ ]; then
	mkdir -p ../out/
fi
if [ -f ../out/cg ]; then
	rm ../out/cg
fi

clang \
	-Wall \
	-O0 \
	-o ../out/cg \
	../src/cg.c
