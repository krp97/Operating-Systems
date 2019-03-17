#!/bin/bash

if [ $# -ne 1 ]; then
	echo 'Invalid number of arguments.'
else
	for plik in `ls -A -1 $1`; do
		if [ ! -x "$1/$plik" ]; then
			if [ -d "$1/$plik" ] && [ ! "$(ls -A $1/$plik)" ]; then
				rmdir "$1/$plik"
			elif [ ! -d "$1/$plik" ]; then
				rm "$1/$plik"
			fi	
		fi
	done
fi

