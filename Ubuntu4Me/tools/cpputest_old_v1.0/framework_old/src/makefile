TOP = ../
include $(TOP)/build/defs.mk

LOCAL_INCLUDES = -I.

SRCS = hello.c 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIB = libKavon.a

BUILDTARGETS = $(DEPENDS) $(LIB) $(EXE)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(LIB)
