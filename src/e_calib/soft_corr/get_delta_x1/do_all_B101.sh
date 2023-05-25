#!/bin/sh

for cut in run2075_cut1b.root  run2075_cut2b.root  run2075_cut3b.root  run2075_cut4b.root  run2075_cut5b.root  run2075_cut6b.root  run2075_cut7b.root
do
	for B in 1.8 1.4 1.0 0.6 0.2 -0.2 -0.6 -1.0 -1.4 -1.8 -2.2
	do
		min_b=`echo $B | awk '{print $1-0.2}'`
		max_b=`echo $B | awk '{print $1+0.2}'`
		echo "min_b: $min_b, max_b: $max_b"
		./get_peak_cen.py ../soft_corr2/root_file_a_corr/run2075_2078_a_corr.root ../soft_corr2/root_file_a_corr/$cut $min_b $max_b
	done
done
