#!/usr/bin/tclsh
# 1st
puts "1st"
puts "Hello World!\n"
# 2nd: var
puts "2nd: var"
set Name "Kavon"
puts "Hello $Name!\n"
# 3th: computation1
puts "3th: computation1"
set y [expr 100+1]
puts "y=100+1=$y\n"
# 4th: computation2
puts "4th: computation2"
incr y -2
puts "y=y-2=$y"
puts {sin(y)=[expr sin($y)]}; # {} disables [] and $
puts "sin(y)=[expr sin($y)]\n"
# 5th: format
puts "5th: format"
set z [format "formatted sin(y) is %-10.5f" [expr sin($y)]]
puts "$z\n"
# 6th: list (http://noyesno.net/page/tcltk/list)
puts "6th: list"
set list_a [list a b c d e];    # 用list命令来创建一个List
set list_b "a b c d e";         # 空白字符分割的字符串也可以用作List
set list_c {a b c d e};         # 同上，只是分组用大括号而不是双引号
set list_d [list a b {c d} e];  # 有时候List的元素本身中就有空白字符的
set list_e {a b {c d} e};       # 同上

set list_f [split "a,b,c,d,e" ","]; # 很多时候需要把字符串分割为List
set str    [join $list_c ","];      # 或者把List组合成一个字符串

foreach i [list 1 2 3 4] {
    puts $i
}

set a [lindex $list_a 0]
puts "first element is $a"
set e [lindex $list_a end]
puts "last element is $e"
set length [llength $list_a]
puts "list length is $length\n"

# 7th: array (http://noyesno.net/page/tcltk/array)
puts "7th: array"
array set kavon [list a 1 b 2]
set kavon(c) 3

set v1 [lindex [array get kavon b] 1]
set v2 $kavon(b)
foreach {k j} [array get kavon *] {
	puts "$k => $j"
}
puts "v1=$v1, v2=$v2\n"

# 8th: process (http://noyesno.net/page/tcltk/proc)
puts "8th: proc"
proc // {args} {
	puts "[lindex $args 1]"
}
// a b c

proc kavon_proc {{param "Hello Kavon!"}} {
	puts "$param"
}
kavon_proc
kavon_proc "Hello Colin!\n"

# 9th: flow (http://noyesno.net/page/tcltk/flow)
puts "9th: flow - if/switch, for/foreach/while"
set var 10
if {$var > 8} {
	puts "I'm here"
} else {
	puts "I'm not here"
}

set options "option_c"
switch $options {
	option_a {puts "option a"}
	option_b {puts "option b"}
	default  {puts "option by default"}
}

for {set i 0} {$i<2} {incr i} {
    puts "Loop for $i"
}

set i 0
while {$i<2} {
    puts "Loop while $i"
	incr i
}

foreach item [list h i j k] {
    puts "This is $item"
}
