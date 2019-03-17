#!/bin/bash

if [ $# -ne 2 ]; then
	echo 'Nieprawidłowa liczba argumentów.'
elif [ ! -d $1 ] && [ ! -d $2 ]; then
	echo 'Podane argumenty nie są katalogami.'
else
	for plik in `ls -A1`; do
		if [ ! -d "$1/$plik" ] && [ -f "$2/$plik" ]; then
			if [ ! -d "$2/$plik" ]; then
				rm "$1/$plik"
			fi
		fi
	done
fi
