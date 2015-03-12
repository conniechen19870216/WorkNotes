TOP = ../..
include $(TOP)/build/defs.mk

EXE = kavon

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

KAVON_LIB  = $(TOP)/main/rhx86/libkavonMain.a
KAVON_LIB1 = $(TOP)/src1/rhx86/libkavonSrc1.a
KAVON_LIBS = $(KAVON_LIB) $(KAVON_LIB1)

BUILDTARGETS = $(DEPENDS) $(EXE)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(EXE): $(KAVON_LIBS)
	$(CC) -g -o $(EXE) $(KAVON_LIBS)
	-cp $(EXE) $(TOP)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(EXE)
