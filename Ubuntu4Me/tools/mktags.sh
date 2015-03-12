#!/bin/bash
###############################################################################
# $Source: /cvs/cvsroot/Repository/TelicaRoot/components/mktags.sh,v $
# $RCSfile: mktags.sh,v $
#
# Author: gdixit <Gaurav Dixit>
# Date : Mar.10.2003
#
# DISCRIPTION:-
# ============
# This script will make C-tags file, for emacs/vi editor based upon the 
# argument given. (default is emacs) 
# It should be called from TelicaRoot/components dir to make tags 
# $ ./mktags.sh [for emacs tags]
# $ ./mktags.sh vi [for Vi tags]
# and will create file 'TAGS' (in case of emacs) or tags (in case of Vi)
# in 'TelicaRoot/components' dir.
#
# HOW TO select tag file from Vi Editor on the fly.
# ================================================
#  Add following statement in your '~/.vimrc'  file. You must alias vi to vim
#  as vi does not have these functions.
#
#  if (match(getcwd(), "/TelicaRoot/components") != -1)
#         let &tags = substitute(getcwd(), "/TelicaRoot/components/.*", "/TelicaRoot/components/tags", "")
#  endif
#
#  if you start Vi from a sandbox, and that sandbox contains 'tags' file,
#  that file will be used for that session of Vi. If no tag file
#  exists in sandbox, no harm done.
#
# HOW TO select tag file from emacs Editor on the fly.
# ===================================================
# I don't know.
#
###############################################################################
TAGS_EXE='/usr/bin/ctags'
TAGS_OPT="--append=yes --if0=no --langmap=c:.c.x.h.C.X.H --sort=yes"
MOREOPTS=
DIR_LIST='dir_list.txt'
COLORS=YES     # make it NO if you don't want colors

# Department of Cleaning
function cleanup()
{
   tput init
   echo -n  ${B_NR}
   if [ "$1" -eq 0 ]
   then
         trap # reset the trap before calling exit 
         exit 0
   fi
   if [ "$2" = "vi" -o "$2" = "VI" -o "$2" = "Vi" -o "$2" = "vI" ]
   then
      echo "Interrupt rcvd...."
      \rm -vrf tags
   else
      echo -n "Interrupt rcvd...."    
      \rm -vrf TAGS
   fi
   trap # reset the trap before  calling exit
   exit 1
}

trap 'cleanup 0 $1' EXIT
trap 'cleanup 1 $1' SIGHUP
trap 'cleanup 2 $1' SIGINT
trap 'cleanup 3 $1' SIGQUIT

# Rang Birangee:- fill some color in life
if [ "${COLORS}" = YES -a "$TERM" = xterm ];
then
   B_BL='[41m[37m'  # Background Blue 
   B_GR='[42m[37m'  # Background Green 
   B_RD='[44m[37m'  # Background Red 
   B_PK='[45m[37m'  # Background Pink 
   B_NR='[!p[?3;4l[4l>[!p[?3;4l[4l>'    # Background Normal, double def, so in case of 'cat' bg/fg not change  
   B_NR='[40m[37m'  # Background Normal 
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


# make decision for emacs or vi
if [ "$1" = "vi" -o "$1" = "VI" -o "$1" = "Vi" -o "$1" = "vI" ] 
then
   \rm -rf tags   # remove any existing tags file  
   echo -n =====================
   echo -n ${B_BL} Making tags for${B_RD} VI Editor ${B_NR}
   echo    =====================
else
   \rm -rf TAGS   # remove any existing tags file  
   echo -n =======================
   echo -n ${B_BL} Making tags for${B_RD} Emacs ${B_NR}
   echo    =======================
   EMACS_TAG='-e'
fi

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
            ${TAGS_EXE} ${TAGS_OPT} ${MORE_OPTS} ${EMACS_TAG} ${FINEL_DIR}/*
         fi
         if [ -n "${FAST_BUILD}" ];then
            echo -n "^" # if want to print dir name =>  echo "${VIRT_SANDBOX}/TelicaRoot/components/${FINEL_DIR}"
            ${TAGS_EXE} ${TAGS_OPT} ${MORE_OPTS} ${EMACS_TAG} ${VIRT_SANDBOX}/TelicaRoot/components/${FINEL_DIR}/*
         fi
      fi
   done
} < ${DIR_LIST} 

echo
echo -n =========================
echo -n ${B_PK} Done making tags ${B_NR}
echo    ==========================

