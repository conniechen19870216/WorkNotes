#!/usr/bin/python
#coding:utf-8

from __future__ import division

x = int(raw_input("please input x: "));
y = int(raw_input("please input y: "));

operator = {'+':x+y, '-':x-y, '*':x*y, '/':x/y};

print operator.get('+')
print operator.get('-')
print operator.get('*')
print operator.get('/')
