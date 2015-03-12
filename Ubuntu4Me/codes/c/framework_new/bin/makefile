TOP = ..
include $(TOP)/build/defs.mk

EXE = kavon
TEST_EXE = kavonTest

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

KAVON_LIB  = $(TOP)/main/rhx86/libkavonMain.a
KAVON_LIB1 = $(TOP)/src1/rhx86/libkavonSrc1.a
KAVON_LIBS = $(KAVON_LIB) $(KAVON_LIB1)

KAVON_TEST_LIBS = $(TOP)/AllTest/rhx86/libkavonAllTest.a \
				  $(TOP)/Test/rhx86/libkavonTest.a \
                  $(CPPUTEST_HOME)/lib/libCppUTest.a \
                  $(CPPUTEST_HOME)/lib/libCppUTestExt.a
KAVON_TEST_LIBS += $(KAVON_LIB1)

BUILDTARGETS = $(DEPENDS) $(EXE)_final

include $(TOP)/build/rules.mk

$(EXE)_final: $(EXE) $(TEST_EXE)

CC = $(GPP)

$(EXE): $(KAVON_LIBS)
	$(CC) -o $(EXE) $(KAVON_LIBS)

$(TEST_EXE):
	$(CC) -o $(TEST_EXE) $(KAVON_TEST_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE) $(EXE)
