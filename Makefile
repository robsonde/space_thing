#
# simple makefile!
#

TARGET := space_thing
CSRC := $(shell find . -iname '*.c')
LIBS := gl
CFLAGS := -std=gnu99 -O2 -pipe -Wall -Wextra -Werror
LDFLAGS := -lglut

include common.mk
