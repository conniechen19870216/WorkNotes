#!/usr/bin/python

x =''

while x != 'q':
	print 'go ahead'
	x = raw_input('please enter sth, q for quit:');
	if not x:
		break
else:
	print 'ending.....'
