#!/usr/bin/env python

"""
define a multiplication table
"""

# define multi_table function
def multi_table(x, y):
	if (x <= y):
		print x, '*', y, '=', x*y, '\t', # use , to avoid printing carriage return
		if (y == 9):
			print '\n'

# call multi_table function
[multi_table(x, y) for x in range(1, 10) for y in range(1, 10)]
