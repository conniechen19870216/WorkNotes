#!/bin/bash

KERNEL=$PWD
#generate the cscope.files
find $KERNEL    \
    -path "$KERNEL/arch/*" ! -path "$KERNEL/arch/arm*" -prune -o    \
	-path "$KERNEL/arch/mach-*" ! -path "$KERNEL/arch/mach-pnx8181*" -prune -o  \
	-path "$KERNEL/include/asm-*" ! -path "$KERNEL/include/asm-arm*" -prune -o  \
	-path "$KERNEL/include/asm-arm/arch-*" ! -path "$KERNEL/include/asm-arm/arch-pnx8181*" -prune -o    \
	-path "$KERNEL/drivers/atm*" -prune -o  \
	-path "$KERNEL/drivers/accessibility*" -prune -o    \
	-path "$KERNEL/drivers/bluetooth*" -prune -o    \
	-path "$KERNEL/drivers/dca*" -prune -o  \
	-path "$KERNEL/drivers/ieee1394*" -prune -o     \
	-path "$KERNEL/drivers/infiniband*" -prune -o   \
	-path "$KERNEL/drivers/ssb*" -prune -o  \
	-path "$KERNEL/drivers/isdn*" -prune -o     \
	-path "$KERNEL/drivers/lguest*" -prune -o   \
	-path "$KERNEL/drivers/s390*" -prune -o     \
	-path "$KERNEL/drivers/telephony*" -prune -o    \
	-path "$KERNEL/drivers/macintosh*" -prune -o    \
	-path "$KERNEL/drivers/parisc*" -prune -o   \
	-path "$KERNEL/drivers/parport*" -prune -o  \
	-path "$KERNEL/drivers/thermal*" -prune -o  \
	-path "$KERNEL/drivers/oprofile*" -prune -o     \
	-path "$KERNEL/drivers/rapidio*" -prune -o  \
	-path "$KERNEL/scripts*" -prune -o  \
	-path "$KERNEL/Documentation*" -prune -o    \
    -name "*.[chsS]" -print > $KERNEL/cscope.files

cscope -bkq -i $KERNEL/cscope.files

#generate the cppcomplete
ctags -n -f cppcomplete.tags --fields=+ai --C++-types=+p * -L $KERNEL/cscope.files
#Try setting the $CSCOPE_DB environment variable to point to a Cscope database you create, so you won't al
#ways need to launch Vim in the same directory as the database.
export CSCOPE_DB=$KERNEL/cscope.out
