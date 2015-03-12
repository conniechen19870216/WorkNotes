TOP = .
include $(TOP)/build/defs.mk

SUBDIRS = $(TOP)/src1 $(TOP)/main $(TOP)/Test $(TOP)/AllTest $(TOP)/bin
SUBDIR_TARGETS = $(SUBDIRS:%=%.subdir)

BUILDTARGETS = $(SUBDIR_TARGETS) 

CC = $(GCC)

include $(TOP)/build/rules.mk

.PHONY: clean
clean:
	-rm -f $(EXE) $(TEST_EXE)
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean; \
	done
