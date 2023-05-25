/** booking_v1.5_fdeg.c 99/05/15 ***********************************/
/** e114(3He,t) offline analysis ************/
/* for finite angle run */

/** INPUT_REG ***************************************************/
for(j=0;j<16;j++){
  if(eventid[j]!=0) {
    book1(1,(float)j);
  }
}


/** INPUT_REG FCET ***************************************************/
for(j=0;j<16;j++){
  if(fcet_id[j]!=-1) {
    book1(2,(float)j);}
}

/** FERA ***********************************************************/
histid1=10;
for(channel=0;channel<32;channel++){
  if(fera_gr[channel]!=-1){
    book1(histid1+channel,(float)fera_gr[channel]);}
}

/** FERET ***********************************************************/
histid1=50;
for(channel=0;channel<32;channel++){
  if(feret_gr[channel]!=-1){
    book1(histid1+channel,(float)feret_gr[channel]);}
}

/** Trig. Scinti. ***************************************************/
book1(180,(float)pos_scinti);
book1d(181,(float)particleid);
book1(182,(float)pos_scinti2);
book1d(183,(float)particleid2);
if(good_pid2)book1d(184,(float)particleid);
if(good_pid1)book1d(185,(float)particleid2);
book2d(200,vdcpos[9][0],fera_gr[0]);
book2d(201,vdcpos[9][0],fera_gr[1]);
book2d(202,vdcpos[9][0],fera_gr[2]);
book2d(203,vdcpos[9][0],fera_gr[3]);
book2d(204,vdcpos[9][0],particleid);
book2d(205,vdcpos[9][0],particleid2);


/** 3377VDC *********************************************************/

histid1=90;
/** GR_FR_X **/
histid2=histid1;
for(k=0;k<grfx.wire;k++) {  
  book1(histid2,(float)grfx.channel[k]);
  book1(histid2+4,(float)grfx.time[k]);
}

j=19;
for(k=0;k<grfx.nclu;k++){
  if(grfx.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grfx.nhit[k]);
}

j=19;
if(grfx.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grfx.nclu);

if(grfx.good==1) {j=0;book1(histid2+16,(float)j);}
for(k=1;k<grfx.posid[0]+1;k++){
  book1(histid2+16,(float)grfx.posid[k]);
}

for(k=0;k<grfx.wire;k++) {
  book1d(histid2+20,grfx.table[grfx.time[k]]);
}

if(grfx.good==1){
  book1d(histid2+24,grfx.pos[0]);
  book1d(histid2+28,grfx.dis);
  book1d(histid2+33,grfx.dis2); /*tmp*/
}
     
/** GR_FR_U **/
histid2=histid1+1;
for(k=0;k<grfu.wire;k++) {  
  book1(histid2,(float)grfu.channel[k]);
  book1(histid2+4,(float)grfu.time[k]);
}

j=19;
for(k=0;k<grfu.nclu;k++){
  if(grfu.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grfu.nhit[k]);
}

j=19;
if(grfu.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grfu.nclu);

if(grfu.good==1) {j=0;book1(histid2+16,(float)j);}
for(k=1;k<grfu.posid[0]+1;k++){
  book1(histid2+16,(float)grfu.posid[k]);
}
     
for(k=0;k<grfu.wire;k++) {
  book1d(histid2+20,grfu.table[grfu.time[k]]);
}

if(grfu.good==1){
  book1d(histid2+24,grfu.pos[0]);
  book1d(histid2+28,grfu.dis);
  book1d(histid2+33,grfu.dis2); /*tmp*/
}

/** GR_RE_X **/
histid2=histid1+2;
for(k=0;k<grrx.wire;k++) {  
  book1(histid2,(float)grrx.channel[k]);
  book1(histid2+4,(float)grrx.time[k]);
}

j=19;
for(k=0;k<grrx.nclu;k++){
  if(grrx.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grrx.nhit[k]);
}

j=19;
if(grrx.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grrx.nclu);

if(grrx.good==1) {j=0;book1(histid2+16,(float)j);}
for(k=1;k<grrx.posid[0]+1;k++){
  book1(histid2+16,(float)grrx.posid[k]);
}

for(k=0;k<grrx.wire;k++) {
  book1d(histid2+20,grrx.table[grrx.time[k]]);
}

if(grrx.good==1){
  book1d(histid2+24,grrx.pos[0]);
  book1d(histid2+28,grrx.dis);
  book1d(histid2+33,grrx.dis2); /*tmp*/
}

/** GR_RE_U **/     
histid2=histid1+3;
for(k=0;k<grru.wire;k++) {  
  book1(histid2,(float)grru.channel[k]);
  book1(histid2+4,(float)grru.time[k]);
}

j=19;
for(k=0;k<grru.nclu;k++){
  if(grru.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grru.nhit[k]);
}

j=19;
if(grru.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grru.nclu);

if(grru.good==1) {j=0;book1(histid2+16,(float)j);}
for(k=1;k<grru.posid[0]+1;k++){
  book1(histid2+16,(float)grru.posid[k]);
}

for(k=0;k<grru.wire;k++) {
  book1d(histid2+20,grru.table[grru.time[k]]);
}

if(grru.good==1){
  book1d(histid2+24,grru.pos[0]);
  book1d(histid2+28,grru.dis);
  book1d(histid2+33,grru.dis2); /*tmp*/
}

if(good_vdc==1){j=0;book1(histid1+32,(float)j);}
else {
  for(k=1;k<vdc_rayid[0]+1;k++){
    book1(histid1+32,(float)vdc_rayid[k]);
  }
}


if(good_vdc==1){
  histid2=210;

  book2d(histid2,(double)feret_gr[4],theta_x[1]);
  book2d(histid2+1,rf_c,theta_x[1]);
  book2d(histid2+2,rf_c,vdcpos10_c[0]);
  book2d(histid2+3,rf_cc,vdcpos10_c[0]);

  histid1=130;histid2=300;
  book1d(histid1,vdcpos[9][0]);
  book1d(histid1+1,vdcpos[9][1]);
  book1d(histid1+2,theta_x[0]);
  book1d(histid1+3,theta_y[0]);
  book1d(histid1+4,vdcpos10_c[0]);
  book1d(histid1+5,vdcpos10_c[1]);
  book1d(histid1+6,theta_x[1]);
  book1d(histid1+7,theta_y[1]);
  book1d(histid1+8,rf_cc);

  book2d(histid2,vdcpos[9][0],vdcpos[9][1]);
  book2d(histid2+1,vdcpos[9][0],theta_x[0]);
  book2d(histid2+2,theta_x[0],theta_y[0]);
  book2d(histid2+3,vdcpos[9][0],theta_x[1]);
  book2d(histid2+4,vdcpos10_c[0],theta_x[1]);
  book2d(histid2+5,vdcpos10_c[0],theta_y[1]);
  book2d(histid2+6,theta_x[1],theta_y[1]);
  book2d(histid2+7,vdcpos10_c[1],theta_x[1]);
  if(good_pid){
    book2d(histid2+10,theta_x[1],vdcpos[9][0]);
    book2d(histid2+11,theta_x[1],vdcpos10_c[0]);
  }
}

/* theta-c cut *********/
if(good_vdc==1 && good_pid){
  histid1=140;histid2=150;
  if(theta_x[1]<0.0 && theta_x[1]>-1.0){
    book1d(histid1,vdcpos10_c[0]);
    book1d(histid1+1,theta_x[1]);
    book1d(histid2,ex_energy);
  }
  else{
    if(theta_x[1]>0.0 && theta_x[1]<1.0){
      book1d(histid1+2,vdcpos10_c[0]);
      book1d(histid1+3,theta_x[1]);
      book1d(histid2+1,ex_energy);
    }
  }
}

if(good_pid){
  histid1=190;
  if(grfx.good==1) {j=0;book1(histid1,(float)j);}
  for(k=1;k<grfx.posid[0]+1;k++){
    book1(histid1,(float)grfx.posid[k]);
  }
  if(grfu.good==1) {j=0;book1(histid1+1,(float)j);}
  for(k=1;k<grfu.posid[0]+1;k++){
    book1(histid1+1,(float)grfu.posid[k]);
  }
  if(grrx.good==1) {j=0;book1(histid1+2,(float)j);}
  for(k=1;k<grrx.posid[0]+1;k++){
    book1(histid1+2,(float)grrx.posid[k]);
  }
  if(grru.good==1) {j=0;book1(histid1+3,(float)j);}
  for(k=1;k<grru.posid[0]+1;k++){
    book1(histid1+3,(float)grru.posid[k]);
  }
  if(good_vdc==1){j=0;book1(histid1+5,(float)j);}
  else {
    for(k=1;k<vdc_rayid[0]+1;k++){
      book1(histid1+5,(float)vdc_rayid[k]);
    }
  }
}


if(good_vdc){
  book1d(400,grfx.kai);
  book1d(401,grfu.kai);
  book1d(402,grrx.kai);
  book1d(403,grru.kai);

  book2d(410,grfx.dis,grfx.dis2);
  book2d(411,grfu.dis,grfu.dis2);
  book2d(412,grrx.dis,grrx.dis2);
  book2d(413,grru.dis,grru.dis2);
  book2d(470,grfx.dis,grfx.dis3);
  book2d(471,grfu.dis,grfu.dis3);
  book2d(472,grrx.dis,grrx.dis3);
  book2d(473,grru.dis,grru.dis3);

  book1d(414,grfx.dis-grfx.dis2);
  book1d(415,grfu.dis-grfu.dis2);
  book1d(416,grrx.dis-grrx.dis2);
  book1d(417,grru.dis-grru.dis2);
  book1d(474,grfx.dis-grfx.dis3);
  book1d(475,grfu.dis-grfu.dis3);
  book1d(476,grrx.dis-grrx.dis3);
  book1d(477,grru.dis-grru.dis3);

  book1d(480,grfx.kai2);
  book1d(481,grfu.kai2);
  book1d(482,grrx.kai2);
  book1d(483,grru.kai2);
  if(grfx.kai2<0.4)  book1d(484,grfx.dis3);
  if(grfu.kai2<0.4)  book1d(485,grfu.dis3);
  if(grrx.kai2<0.4)  book1d(486,grrx.dis3);
  if(grru.kai2<0.4)  book1d(487,grru.dis3);
  book1d(488,grfx.dis3);
  book1d(489,grrx.dis3);

  if(grfx.kai<0.4){
    book1d(420,grfx.kai);
    book2d(430,grfx.dis,grfx.dis2);
    book1d(434,grfx.dis-grfx.dis2);
    book1d(440,grfx.dis2);
    book1d(444,grfx.dis);
    for(k=0;k<grfx.wire;k++) book1(450,(float)grfx.time[k]);
    book1(460,grfx.wire);
  }
  for(k=0;k<grfx.wire;k++) book1(454,(float)grfx.time[k]);
  book1(464,grfx.wire);

  if(grfu.kai<0.4){
    book1d(421,grfu.kai);
    book2d(431,grfu.dis,grfu.dis2);
    book1d(435,grfu.dis-grfu.dis2);
    book1d(441,grfu.dis2);
    book1d(445,grfu.dis);
    for(k=0;k<grfu.wire;k++) book1(451,(float)grfu.time[k]);
    book1(461,grfu.wire);
  }
  for(k=0;k<grfu.wire;k++) book1(455,(float)grfu.time[k]);
  book1(465,grfu.wire);

  if(grrx.kai<0.4){
    book1d(422,grrx.kai);
    book2d(432,grrx.dis,grrx.dis2);
    book1d(436,grrx.dis-grrx.dis2);
    book1d(442,grrx.dis2);
    book1d(446,grrx.dis);
    for(k=0;k<grrx.wire;k++) book1(452,(float)grrx.time[k]);
    book1(462,grrx.wire);
  }
  for(k=0;k<grrx.wire;k++) book1(456,(float)grrx.time[k]);
  book1(466,grrx.wire);

  if(grru.kai<0.4){
    book1d(423,grru.kai);
    book2d(433,grru.dis,grru.dis2);
    book1d(437,grru.dis-grru.dis2);
    book1d(443,grru.dis2);
    book1d(447,grru.dis);
    for(k=0;k<grru.wire;k++) book1(453,(float)grru.time[k]);
    book1(463,grru.wire);
  }
  for(k=0;k<grru.wire;k++) book1(457,(float)grru.time[k]);
  book1(467,grru.wire);
}

