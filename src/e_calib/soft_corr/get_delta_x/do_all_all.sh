#!/bin/sh

rm -f delta_x.txt
./do_all_B100.sh | grep "peak center: " >> delta_x.txt
./do_all_B101.sh | grep "peak center: " >> delta_x.txt
./do_all_B102.sh | grep "peak center: " >> delta_x.txt
