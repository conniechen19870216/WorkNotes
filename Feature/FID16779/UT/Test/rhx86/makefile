TOP = ../..
include $(TOP)/build/defs.mk

LOCAL_VPATH = ../
LOCAL_INCLUDES = -I$(CPPUTEST_HOME)/include
LOCAL_INCLUDES += -I$(TOP)/src1 -I$(TOP)/sigcom/include

SRCS = digitMapTest.c
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIB = libkavonTest.a

BUILDTARGETS = $(DEPENDS) $(LIB) 

CC = $(GPP)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(LIB)
