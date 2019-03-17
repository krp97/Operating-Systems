#!/bin/bash

if [ $# -ne 2 ]; then
	echo 'Invalid amount of arguments'
else
	for wiersz in `cat $2`; do
		if [ -f $wiersz ] && [ ! -d $wiersz ]; then
		echo $wiersz >> "$1/$2"
		cat "$1/$wiersz" >> "$1/$2"
		fi
	done
fi
