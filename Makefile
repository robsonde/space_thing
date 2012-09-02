#
# simple makefile!
#

TARGET := space_thing
CSRC := $(shell find . -iname '*.c')
LIBS := sdl gl freetype2 ftgl
CFLAGS := -std=gnu99 -O2 -pipe -Wall -Wextra -Werror
LDFLAGS := 

include common.mk
