#!/bin/bash

if [ $# -ne 2 ]; then
	echo 'Invalid amount of arguments.'
else
	for plik in `ls -A -1 $1`;do
		if [ -x "$1/$plik" ] && [ ! -d "$1/$plik" ]; then
			mv "$1/$plik" "$2/$plik"
		fi
	done
fi

