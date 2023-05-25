#!/bin/sh

for i in 2074 2075_2078 2079 2080_2081
do
	./soft_corr ../old_root/run${i}_ang.root root_file_a_corr/run${i}_a_corr.root
done
