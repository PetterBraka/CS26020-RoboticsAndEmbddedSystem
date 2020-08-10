#
# Makefile for Matrix Formula All Code Buggy
#
# The Makefile is licensed under MIT
# and is part of the Pinit project.
#
# Author: Sindre Aalhus
# Since: 29.01.2019
#

OS = Linux

VERSION = 1.0.0
CC	= /opt/microchip/xc16/v1.36/bin/xc16-gcc
BTH 	= /opt/microchip/xc16/v1.36/bin/xc16-bin2hex

TARGET 	:= bin/main
PARAM 	:= -c -mcpu=33EP256MU810 -MMD -mno-eds-warn -g -omf=elf -DXPRJ_default=default -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
PARAM2 	:= -mcpu=33EP256MU810 -omf=elf -DXPRJ_default=default  -legacy-libc    -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,,--script=p33EP256MU810.gld,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io

SRCDIR := src
OBJDIR := obj
LIBDIR := lib

SRCS := $(shell find $(SRCDIR) -name "*.c")
HDRS := $(shell find $(SRCDIR) -name "*.h")
LIBS := $(shell find $(LIBDIR) -name "*.o")
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(TARGET): $(OBJS)
	@mkdir -p bin
	@echo xc16 -o $@ $^ $(LIBS)
	@$(CC) -o $@ $^ $(LIBS) $(PARAM2)
	@echo xc16-bin2hex -o $@ -a -omf=elf
	@$(BTH) $@ -a -omf=elf

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDRS)
	@mkdir -p $(shell dirname $@)
	@echo xc16 $@ -o $< -c
	@$(CC) $(@:$(OBJDIR)/%.o) $< -o $@  $(PARAM)

clean:
	rm -f $(OBJDIR)/*.o

all: clean $(TARGET)

load: bin/main.hex
	faload $<

