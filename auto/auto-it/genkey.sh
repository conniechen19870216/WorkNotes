#!/bin/ksh

genkey.exp
scp2vm10 ~/.ssh/id_dsa.pub /tmp
vm10run "cat /tmp/id_dsa.pub >> /root/.ssh/authorized_keys"
