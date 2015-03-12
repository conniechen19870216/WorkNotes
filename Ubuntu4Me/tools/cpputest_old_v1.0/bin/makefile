TOP = ..
include $(TOP)/build/defs.mk

TEST_EXE = kavonTest

LOCAL_VPATH = $(TOP)/src
LOCAL_INCLUDES = -I$(CPPUTEST_HOME)/include

SRCS = AllTest.c
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

KAVON_LIB = $(TOP)/src/libKavon.a

KAVON_TEST_LIBS = $(TOP)/Test/libkavonTest.a \
                  $(CPPUTEST_HOME)/lib/libCppUTest.a \
                  $(CPPUTEST_HOME)/lib/libCppUTestExt.a
KAVON_TEST_LIBS += $(KAVON_LIB)

BUILDTARGETS = $(DEPENDS) $(EXE) $(TEST_EXE)

include $(TOP)/build/rules.mk

CC = $(GPP)

$(TEST_EXE): $(OBJS)
	$(CC) -o $(TEST_EXE) $(OBJS) $(KAVON_TEST_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE)
