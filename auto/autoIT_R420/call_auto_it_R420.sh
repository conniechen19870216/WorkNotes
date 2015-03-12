#!/bin/csh
#set vmmode = "comp"
#set vmmode = "dist"
if ( $1 != "comp" && $1 != "dist" && $1 != "mnrg") then
	echo "$0 [comp|dist|mnrg]"
	exit 1
endif


set vmmode = $1
set stamp = `date +%m%d-%H%M`
set logfile = "/home/it40/jenkins_script/autoIT_R420/log/auto-it.log.$vmmode.$stamp"

source /home/it40/jenkins_script/autoIT_R420/auto-it_R420.env

if ( $vmmode == "comp") then
	set itdisp = $ITDISPLAY
else
	set itdisp = $ITDISPLAYD
endif

/home/it40/jenkins_script/autoIT_R420/run_auto_it_R420.dist $vmmode | tee $logfile

grep "IT cases passed" $logfile
set rt="$?"
grep "avoid conflict" $logfile
set rt1="$?"

if ($rt == "0" || $rt1 == "0") then
	echo "INFO: AUTO-IT for $vmmode SUCCESS. The full log is: $logfile ."
	exit 0
else
	echo "ERROR: AUTO-IT for $vmmode failed,please check. The full log is: $logfile ."
	echo "The VNC is $itdisp, password: 123456"
	exit 1
endif
