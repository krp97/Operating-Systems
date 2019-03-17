#!/bin/bash


if [ $# -ne 2 ]; then
	echo 'Invalid amount of arguments'
elif [ ! -d $1 ] || [ ! -d $2 ]; then
	echo 'Param 1 or 2 (or both) are not directories.'
else
	echo 'Gooooooooood'
fi
