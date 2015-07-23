all: sample run gprof_report gcov_report
sample: sample.c
	gcc -pg -fprofile-arcs -ftest-coverage -o $@ $^
run: sample
	./$<
gprof_report: sample gmon.out
	gprof -b $^ > $@
gcov_report: sample.c
	gcov $^
.PHONY: clean
clean:
	-rm -f sample gprof_report sample.c.gcov sample.gcda sample.gcno sysmacros.h.gcov gmon.out
