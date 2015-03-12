TOP = ../../
include $(TOP)/build/defs.mk

LOCAL_VPATH = ../
LOCAL_INCLUDES = -I./

SRCS = kavon_main.c 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIB = libkavonMain.a

BUILDTARGETS = $(DEPENDS) $(LIB)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(LIB)
