#!/bin/sh

for x0 in 66.11 66.12 66.13 66.14 66.15 66.16 66.17 66.18 66.19 66.20 66.21 66.22
do
	chi2=0
	tmp=`./x_to_Ex.py -295.8 $x0 | awk '{print $7}' | awk -F"(" '{print $1}'|awk '{print ($1-10.354)*($1-10.354)}'`
	chi2=`echo "$chi2  $tmp"|awk '{print $1+$2}'`
	tmp=`./x_to_Ex.py -271.2 $x0 | awk '{print $7}' | awk -F"(" '{print $1}'|awk '{print ($1-10.916)*($1-10.916)}'`
	chi2=`echo "$chi2  $tmp"|awk '{print $1+$2}'`
	tmp=`./x_to_Ex.py -189.1 $x0 | awk '{print $7}' | awk -F"(" '{print $1}'|awk '{print ($1-12.806)*($1-12.806)}'`
	chi2=`echo "$chi2  $tmp"|awk '{print $1+$2}'`
	tmp=`./x_to_Ex.py -117.4 $x0 | awk '{print $7}' | awk -F"(" '{print $1}'|awk '{print ($1-14.455)*($1-14.455)}'`
	chi2=`echo "$chi2  $tmp"|awk '{print $1+$2}'`
	echo "x0= $x0, chi2= $chi2"

done

