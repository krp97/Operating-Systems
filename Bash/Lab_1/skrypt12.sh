#!/bin/bash

plikireg=0
foldery=0

for plik in `ls -A1`; do
	if [ -x "$1/$plik" ] && [ ! -d "$1/$plik" ]; then
		let "plikireg++"
	elif [ -x "$1/$plik" ] && [ -d "$1/$plik" ]; then
		let "foldery++"
	fi
done
echo "Pliki regularne: $plikireg"
echo "Foldery: $foldery"
