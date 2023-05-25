#!/bin/sh

for x in `awk '{print $1}' tmp.txt`
do
	./tmp.py $x
done
