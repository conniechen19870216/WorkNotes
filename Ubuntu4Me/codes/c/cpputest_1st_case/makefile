TOP = .
include $(TOP)/build/defs.mk

EXE = kavonTest

LOCAL_INCLUDES = -I$(CPPUTEST_HOME)/include

SRCS = AllTest.c
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIBS = $(TOP)/local/libkavonTest.a \
       $(CPPUTEST_HOME)/lib/libCppUTest.a \
	   $(CPPUTEST_HOME)/lib/libCppUTestExt.a

SUBDIRS = $(TOP)/local
SUBDIR_TARGETS = $(SUBDIRS:%=%.subdir)

BUILDTARGETS = $(DEPENDS) $(SUBDIR_TARGETS) $(EXE)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(EXE)
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean; \
	done
