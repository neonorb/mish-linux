# Makefile for Mish Linux

CSOURCES=mish-linux syscalls
ASOURCES=

LIBS=mish feta

# ----

-include ../make-base/make-base.mk

all: build/mish-linux

build/mish-linux: $(OBJECTS)
	g++ $(OBJECTS)          \
		$(LIBS_FLAGS)       \
		-o build/mish-linux
	
	chmod +x build/mish-linux