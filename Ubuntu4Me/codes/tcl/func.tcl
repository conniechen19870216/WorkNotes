#!/usr/bin/tclsh
set operators [list + - * /]
foreach o $operators {
	proc $o {a b} [list expr "\$a $o \$b"]
}
puts [+ 1 2]
