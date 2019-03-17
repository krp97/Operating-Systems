#!/bin/bash
if [ $# = 1 ]
then
	for var in `ls $1 -1`
	do
		if [ -w $var ]
		then
			mv "$1/$var" "$1$var.old"
		fi
	done
else
	echo 'Invalid amount of arguments.'
fi


			
