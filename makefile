# Makefile for Mish Linux

NAME=mish-linux

CPPSOURCES=mish-linux syscalls console
ASOURCES=

LIBS=mish feta

# ----

-include ../make-base/make-base.mk

CFLAGS+=-fno-rtti -fno-exceptions

-include ../make-base/make-bin.mk