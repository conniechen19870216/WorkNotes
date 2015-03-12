#!/usr/bin/python
#coding:utf-8

from __future__ import division

x = int(raw_input("please input x: "));
y = int(raw_input("please input y: "));

def jia(x, y):
	return x+y

def jian(x, y):
	return x-y

def cheng(x, y):
	return x*y

def chu(x, y):
	return x/y

operator = {'+':jia, '-':jian, '*':cheng, '/':chu};

def calc(x, o, y):
	print operator.get(o)(x, y)

print calc(x, '+', y)
print calc(x, '-', y)
print calc(x, '*', y)
print calc(x, '/', y)
