#!/bin/sh

for cut in run2074_cut1.root  run2074_cut2.root  run2074_cut3.root  run2074_cut4.root  run2074_cut5.root  run2074_cut6.root  run2074_cut7.root
do
	for A in 0.6 0.4 0.2 0 -0.2 -0.4 -0.6 -0.8 -1.0 -1.2 -1.4
	do
		min_a=`echo $A | awk '{print $1-0.1}'`
		max_a=`echo $A | awk '{print $1+0.1}'`
		echo "min_a: $min_a, max_a: $max_a"
		./get_peak_cen.py ../old_root/run2074_ang.root ../old_root/$cut $min_a $max_a
	done
done
