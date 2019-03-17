#!/bin/bash

if [ $# -ne 1 ]; then
	echo 'Invalid amount of arguments.'
else
	counter=1
	for plik in `ls -1 -S`; do
	       mv "$1/$plik" "$1/$plik.$counter"
	       let "counter=counter+1"
       done       
fi

