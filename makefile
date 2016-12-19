# Makefile for Mish Linux

CSOURCES=mish-linux syscalls console
ASOURCES=

LIBS=mish feta

INSTALL=/usr/local/bin/mish

# ----

-include ../make-base/make-base.mk

CFLAGS+=-fno-rtti -fno-exceptions

all: build/mish

build/mish: $(OBJECTS)
	@g++ $(OBJECTS)          \
		$(LIBS_FLAGS)       \
		-o build/mish
	
	@chmod +x build/mish

.PHONY:
install: build/mish
	@cp build/mish $(INSTALL)