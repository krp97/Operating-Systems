#!/bin/bash

if [ $# -ne 2 ]; then
	echo 'Invalid amount of arguments'
else
	for plik1 in `ls -A1 $1`; do
		if [ -d $plik1 ]; then
			for plik2 in `ls -A1 $plik1`; do
				mv "$1/$plik1/$plik2" "$1/$plik2"
			done
			rm -r "$1/$plik1"
		fi
	done
fi
