#!/bin/bash

if [ "$#" -ne "2" ]
then
	echo 'Invalid amount of arguments'
else
	pliki=`cat $2`
	for plik in $pliki
	do
		touch "$1/$plik"
	done
fi
