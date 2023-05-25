with the ./angle_cal one can convert the x,theta,y values measured at focal
plane to A and B angles at target. The conversion coefficients are in
../ss_calib/ ( see also the wiki page). The input files are in ../root_files/
Please also note that, during the conversion, cut in PID is also applied:
(LA2*RA2 > 25e3 && X != -10000)

usage: ./angle_cal <input root file> <output root file>
