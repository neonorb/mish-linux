# Makefile for Mish Linux

CSOURCES=mish-linux syscalls
ASOURCES=

LIBS=mish feta

# ----

-include ../make-base/make-base.mk

all: build/mish

build/mish: $(OBJECTS)
	g++ $(OBJECTS)          \
		$(LIBS_FLAGS)       \
		-o build/mish
	
	chmod +x build/mish