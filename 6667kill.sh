#!/bin/sh

ARGS=$(lsof -i :6667 | awk 'NR > 1 {print $2}') 

for arg in $ARGS
do
	kill -9 $arg
done

