#!/bin/bash

if [ $# -ne 1 ]; then
	echo 'Invalid amount of arguments'
else
	for plik in `ls -1 -A $1`; do
		if [ ! -d "$1/$plik" ]; then
			echo "$plik"
			cat "$1/$plik"
		fi
	done
fi

