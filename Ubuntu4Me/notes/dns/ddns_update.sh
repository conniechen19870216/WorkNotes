#!/bin/bash
# 
# Update your Dynamic IP by using BIND 9 's tools
#
###############################################
# History
# 2004/10/27	VBird	First time release
#
##############################################
PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

# 0. keyin your parameters
basedir="/usr/local/ddns"			# working directory
keyfile="$basedir"/"Kweb.+157+50096.key"	# your ddns' key (filename)
ttl=600						# the ttl time ( 10 min. )
outif="ppp0"					# Your interface (connect to internet)
hostname="web.vbird.tsai"			# Your hostname
servername="192.168.1.254"			# The update primary DNS server name (or IP)
showmesg=no                                     # if yes then show messages

# Get your new IP
newip=`ifconfig "$outif" | grep 'inet addr' | \
	awk '{print $2}' | sed -e "s/addr\://"`
checkip=`echo $newip | grep "^[0-9]"`
if [ "$checkip" == "" ]; then
	echo "$0: The interface can't connect internet...."
	exit 1
fi

# check if the DNS is the same with your IP
dnsip=`host $hostname | head -n 1 | awk '{print $4}'`
if [ "$newip" == "$dnsip" ]; then
        if [ "$showmesg" == "yes" ]; then
                echo "$0: The IP is the same with DNS, Don't change it."
        fi
        exit 0
fi

# create the temporal file
tmpfile=$basedir/tmp.txt
cd $basedir
echo "server $servername" 			>  $tmpfile
echo "update delete $hostname A " 		>> $tmpfile
echo "update add    $hostname $ttl A $newip" 	>> $tmpfile
echo "send" 					>> $tmpfile


# send your IP to server
nsupdate -k $keyfile -v $tmpfile

