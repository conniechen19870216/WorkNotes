#!/usr/bin/python
print "---------func1--------"
def func1(name='name', age=20):
	print "%s ------ %s" % (name, age)
	print "name: %s" % name
	print "age : %s" % age 

t = ('kavon', 30)
func1(*t)

print "---------func2.1--------"

def func2(name='name', age=20):
	print "name: %s" % name
	print "age : %s" % age 

d = {'name':'ma', 'age':14}
func2(age=25, name='songchi')

print "---------func2.2--------"
func2(**d)

print "---------func3--------"

def func3(name='name', age=20):
	print "%s ******** %s" % (name, age) 
e = {'name':'song', 'age':32}
func3(**e)
func3(*e)

print "---------func4--------"

def func4(name, *args, **kwargs):
	print name
	print args
	print kwargs

func4(1,2)
func4(1,x=2,y=3)
func4(1,'kavon', 'ma', x=2,y=3)
