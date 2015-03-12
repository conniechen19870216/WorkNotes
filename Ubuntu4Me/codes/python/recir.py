#!/usr/bin/python
def fun(n):
	if n>0:
		return n*fun(n-1)
	else:
		return 1

if __name__ == '__main__':
	print fun(5)
