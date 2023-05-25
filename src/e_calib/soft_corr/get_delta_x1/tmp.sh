#!/bin/sh

for (( i = 0; i < 21; i++ )); do
for B in 1.8 1.4 1.0 0.6 0.2 -0.2 -0.6 -1.0 -1.4 -1.8 -2.2
do
	echo "B: $B"
done
done
