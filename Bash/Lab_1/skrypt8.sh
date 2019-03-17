#!/bin/bash

for plik in `ls -1A $1`; do
	if [ -f "$2/$plik" ]; then
		echo -e "$plik"
	fi
done
