#!/bin/bash
###############################################################################
# $Source: /cvs/cvsroot/Repository/TelicaRoot/components/mkcscope.sh,v $
# $RCSfile: mkcscope.sh,v $
#
# Author: gdixit <Gaurav Dixit>
# Date : Mar.10.2003
#
# DISCRIPTION:-
# ============
# This script will make cscope.out file, which can be used from command line 
# as well as can be connected to text editor (emacs/vi).
# It should be called from TelicaRoot/components dir to make cscope.out 
# $ ./mkcscope.sh
# and will create cscope.out in 'TelicaRoot/components' dir.
#
# HOW TO USE cscope.out FROM COMMAND LINE
# =======================================
#
# define following function and SAND_BOX in your .bashrc file
#
#  # ============================================================
#  # this function will choose cscope.out file based upon from 
#  # which sandbox it is being called using 'pwd'. And then 
#  # will prefix the sandbox path and start cscope.
#  function cs ()
#  {
#     # see if we are in sandbox
#     if [ `echo ${PWD} |grep -c "TelicaRoot/component"` -ne 0 ]
#     then
#        SAND_BOX=`echo ${PWD}|sed 's/TelicaRoot\/components\/.*/TelicaRoot\/components/'`
#        if [ -f ${SAND_BOX}/cscope.out ]
#        then
#           cscope -d -P ${SAND_BOX} -f ${SAND_BOX}/cscope.out
#        else
#           echo "cscope.out does not exist, in this sandbox, use mkcscope.sh to create one"
#        fi
#     else
#        echo "You are outside the sandbox"
#     fi
#  }
#  # call this function from shell and see the magic
###############################################################################
CS_EXE='cscope' # it shoule be => '/usr/local/bin/cscope'
CS_OPT='-b -u -q'
MOREOPTS=
FILELIST=`mktemp /tmp/${USER}.fileList.XXXXXX`
DIR_LIST='dir_list.txt'
COLORS=YES     # make it NO if you don't want colors

# Department of Cleaning
function cleanup()
{
   tput init
   echo -n  ${B_NR}
   \rm -rf ${FILELIST}
   if [ "$1" -eq 0 ]
   then
      trap # reset the trap before calling exit 
      exit 0;
   fi
   echo "Interrupt rcvd...."
   \rm -rfv ncscope.out cscope.*
   trap # reset the trap before  calling exit
   exit 1;
}

trap 'cleanup 0' EXIT
trap 'cleanup 1' SIGHUP
trap 'cleanup 2' SIGINT
trap 'cleanup 3' SIGQUIT

# Rang Birangee:- fill some color in life
if [ "${COLORS}" = YES -a "$TERM" = xterm ];
then
   B_BL='[41m[37m'  # Background Blue 
   B_GR='[42m[37m'  # Background Green 
   B_RD='[44m[37m'  # Background Red 
   B_PK='[45m[37m'  # Background Pink 
   B_NR='[!p[?3;4l[4l>[!p[?3;4l[4l>'    # Background Normal, double def, so in case of 'cat' bg/fg not change  
fi

# see if we are in correct dir to run it.
if [ $(basename $(pwd)) != components ]; then
   echo -n ${B_BL}Please run from ${B_RD}
   echo -n \$send_box/TelicaRoot/components
   echo  ${B_BL} directory ${B_NR}
   exit 1
fi

#see if dir_list.txt file is present
if [ ! -f ${DIR_LIST} ]; then
   echo ${B_BL}File${B_RD} ${DIR_LIST} ${B_BL} does not exist!!! ${B_NR}
   exit 1
fi

rm -rfv signaling/cc/fic/fc_bdy1.c
echo -n =========================
echo -n ${B_BL} Making${B_RD} cscope.out ${B_NR}
echo    =========================

\rm -rf cscope.out ncscope.out # remove any existing cscope file

# This is main loop
{
   if [ -n "${FAST_BUILD}" ];then
      echo ${B_GR} You are in FAST_BUILD mode ${B_NR}
   fi
   while read DIRNAME;
   do
      FINEL_DIR=`echo "${DIRNAME}"|sed 's/\#.*//'|sed 's/[[:space:]]//g'`
      if [ -n "${FINEL_DIR}" ];then
         if [ -d "${FINEL_DIR}" ];then
            echo -n "." # if want to print dir name =>  echo "${FINEL_DIR}"
            \ls -1 ./${FINEL_DIR}/*.[chx] >> ${FILELIST} 2> /dev/null
         fi
         if [ -n "${FAST_BUILD}" ];then
            echo -n "^" # if want to print dir name =>  echo "${VIRT_SANDBOX}/TelicaRoot/components/${FINEL_DIR}"
            \ls -1 ${VIRT_SANDBOX}/TelicaRoot/components/${FINEL_DIR}/*.[chx] >> ${FILELIST} 2> /dev/null
         fi
      fi
   done
} < ${DIR_LIST} 

# This line will make actual cscope.out..will take long time
echo
echo "Building..it may take few minutes.."
${CS_EXE} ${CS_OPT} -i ${FILELIST}

echo -n ======================
echo -n ${B_PK} Done making cscope.out ${B_NR}
echo    =======================
echo

