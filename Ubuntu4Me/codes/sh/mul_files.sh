#!/bin/bash
# Configure your own parametes here
PATTERN=*.sh
KEYWORD=9721
REPLACE=9621
NAMESFILE=namesfile

# Action!
find . -type f -name '*.sh' | grep $KEYWORD | xargs | tee $NAMESFILE
COLUMNS=`awk '{print NF}' $NAMESFILE`
sed s/$KEYWORD/$REPLACE/g $NAMESFILE | tee -a $NAMESFILE
for (( i=1; i<=$COLUMNS; i++ )); do
	cut -f$i -d" " $NAMESFILE | xargs -n 2 mv
done

# Delete temp files
rm $NAMESFILE
