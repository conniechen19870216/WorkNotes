all: $(BUILDTARGETS)

%.d: %.c
	$(CC) -MM $(CFLAGS) $< \
	| sed 's/\($*\)\.o[ :]/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.subdir:
	@cd $(patsubst %.subdir,%,$@); \
	make

-include $(DENPENDS)
