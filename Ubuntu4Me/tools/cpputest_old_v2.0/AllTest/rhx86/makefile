TOP = ../../
include $(TOP)/build/defs.mk

LOCAL_VPATH = ../
LOCAL_INCLUDES = -I$(CPPUTEST_HOME)/include

SRCS = AllTest.c 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIB = libkavonAllTest.a

BUILDTARGETS = $(DEPENDS) $(LIB)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(LIB)
