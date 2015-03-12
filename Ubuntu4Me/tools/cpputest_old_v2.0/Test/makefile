TOP = ../
include $(TOP)/build/defs.mk

SUBDIRS = ./rhx86
SUBDIR_TARGETS = $(SUBDIRS:%=%.subdir)

BUILDTARGETS = $(SUBDIR_TARGETS) 

CC = $(GPP)

include $(TOP)/build/rules.mk

.PHONY: clean
clean:
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean; \
	done
