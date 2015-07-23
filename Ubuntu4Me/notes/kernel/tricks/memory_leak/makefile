all: libtrace.so kavon trace_log

DIR=~/kernel/tricks/memory_leak
# step 1: create memory trace library (e.g. libtrace.so)
# Command: gcc -nostartfiles -fPIC -shared trace.c -o libtrace.so
libtrace.so: trace.c
	gcc -nostartfiles -fPIC -shared $< -o $@

# step 2: create testing binary (e.g. kavon)
# Command: gcc -g2 -o kavon main.c
kavon: main.c
	gcc -g2 -o $@ $<

# record the memory trace log
# Command: LD_PRELOAD=~/kernel/tricks/memory_leak/libtrace.so MALLOC_TRACE=~/kernel/tricks/memory_leak/trace.log ./kavon
trace_log:
	LD_PRELOAD=${DIR}/libtrace.so MALLOC_TRACE=${DIR}/trace.log ./kavon

.PHONY: clean
clean: 
	-rm -f libtrace.so kavon
