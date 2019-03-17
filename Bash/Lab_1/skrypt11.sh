#!/bin/bash

if [ $# -ne 1 ]; then
	echo 'Invalid amount of arguments'
else
	for plik in `ls -A1 $1`; do
		if [ -w $plik ]; then
			touch $plik
		fi
	done
fi
