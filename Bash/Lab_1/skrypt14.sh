#!/bin/bash

if [ $# -ne 1 ]; then
	echo 'Invalid amount of arguments'
else
	for plik in `ls -1`; do
		if [ -d "$1/$plik" ]; then
			chmod u-x "$1/$plik"
		else
			chmod u+w "$1/$plik"
		fi
	done
fi
