TOP = ../..
include $(TOP)/build/defs.mk

LOCAL_VPATH = ../
LOCAL_INCLUDES = -I$(TOP)/sigcom/include -I$(TOP)/src1 

SRCS = kavon_main.c 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIB = libkavonMain.a

BUILDTARGETS = $(DEPENDS) $(LIB)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(LIB)
