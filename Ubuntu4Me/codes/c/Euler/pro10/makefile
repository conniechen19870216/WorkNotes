.PHONY: all clean

ifndef ATCA
all clean:
	make ATCA=t $@
endif

ifdef ATCA
all:
	@gcc -o kavon pro10.c -lm
clean:
	@-rm kavon
endif
