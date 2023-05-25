#!/bin/sh

for i in 2074 2075_2078 2079 2080_2081
do
	./soft_corr ../soft_corr2/root_file_a_corr/run${i}_a_corr.root root_file_ab_corr/run${i}_ab_corr.root
done
