TOP = ../..
include $(TOP)/build/defs.mk

TEST_EXE = kavonTest

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

KAVON_LIB1 = $(TOP)/src1/rhx86/libkavonSrc1.a

KAVON_TEST_LIBS = $(TOP)/AllTest/rhx86/libkavonAllTest.a \
				  $(TOP)/Test/rhx86/libkavonTest.a \
                  $(CPPUTEST_HOME)/lib/libCppUTest.a \
                  $(CPPUTEST_HOME)/lib/libCppUTestExt.a
KAVON_TEST_LIBS += $(KAVON_LIB1)

BUILDTARGETS = $(DEPENDS) $(TEST_EXE)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(TEST_EXE):
	$(CC) -g -o $(TEST_EXE) $(KAVON_TEST_LIBS)
	-cp $(TEST_EXE) $(TOP)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE)
