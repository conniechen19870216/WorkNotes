TOP = ..
include $(TOP)/build/defs.mk

LOCAL_INCLUDES = -I$(CPPUTEST_HOME)/include
LOCAL_INCLUDES += -I$(TOP)/local/src

SRCS = kavonTest.c kavonTestMock.c 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIB = libkavonTest.a

SUBDIRS = src
SUBDIR_TARGETS = $(SUBDIRS:%=%.subdir)

BUILDTARGETS = $(DEPENDS) $(SUBDIR_TARGETS) $(LIB) 

CC = $(GPP)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(LIB)
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean; \
	done
