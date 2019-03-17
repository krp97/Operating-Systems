#!/bin/bash

if [ $# -ne 2 ]; then
	echo 'Invalid amount of arguments'
else
	for plik in `ls -A -1 $1`; do
		if [ `stat --format=%s "$1/$plik"` = 0 ]; then
			if [ ! -d "$1/$plik" ]; then
				echo -e $plik >> $2
				rm $plik
			fi		
		fi
	done

fi
