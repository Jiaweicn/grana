/** booking_v1.5_b.c 99/11/03 ***********************************/
/** e247 online analysis ************/

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
 
/**********************************************************************/
/* Booking LAS */
/** FERA ***********************************************************/
if(eventid[LAS_FP_EVENT]||eventid[COIN]){
  histid1=10;
  for(channel=0;channel<12;channel++){
    if(fera_las[channel]){
      book1(histid1+channel,(float)fera_las[channel]);}
  }
  /** FERET ***********************************************************/
  histid1=40;
  for(channel=0;channel<12;channel++){
    if(feret_las[channel*2]){
      book1(histid1+channel,(float)feret_las[channel*2]);}
  }
  book1(52,(float)feret_las[26]);
  book1(53,(float)feret_las[28]);
  book1(54,(float)feret_las[30]);

  
  /** Particle ID and Position ****************************************/
  for(i=0;i<6;i++) {
    if(feret_las[4*i]*feret_las[4*i+2]){
      book1(30+i,(float)particleid_las[i]);
      book1(60+i,(float)pos_scinti_las[i]);
    }
  }
  
  /** 3377VDC *********************************************************/
  histid1=90;

  if(good_pid_las){
    /** LAS_FR_U **/
    histid2=histid1;
    bookvdc(&lasfu,histid2);
    /** LAS_FR_V **/
    histid2=histid1+1;
    bookvdc(&lasfv,histid2);
    /** LAS_RE_U **/     
    histid2=histid1+2;
    bookvdc(&lasru,histid2);
    /** LAS_RE_V **/     
    histid2=histid1+3;
    bookvdc(&lasrv,histid2);

#if 0
    /** LAS_FR_X **/
    histid2=histid1;
    bookvdc(&lasfx,histid2);
    /** LAS_RE_X **/
    histid2=histid1+2;
    bookvdc(&lasrx,histid2);
#endif

    for(k=0;k<26;k++){
      if(las_rayid&(1<<k)) book1(122,(float)k);
    }

    /** MWDC **/
    for(k=0;k<lasfr.wire;k++) {
      book1(123,(float)lasfr.channel[k]);
      book1(124,(float)lasfr.time[k]);
    }
    for(k=0;k<lasrr.wire;k++) {
      book1(125,(float)lasrr.channel[k]);
      book1(126,(float)lasrr.time[k]);
    }
    for(k=0;k<4;k++){
      book2d(127,(float)k,(float)lasfr.nhit[k]);
      book2d(127,(float)(k+4),(float)lasrr.nhit[k]);
      book2d(128,(float)k,(float)lasfr.cclu[k]);
      book2d(128,(float)(k+4),(float)lasrr.cclu[k]);
    }
  }

  histid1=130;histid2=300;
  /*if(good_las==1&&(good_pid_las&0x0002))*/
  if(good_las==1){
    book1d(histid1,laspos[9][0]);
    book1d(histid1+1,laspos[9][1]);
    book1d(histid1+2,theta_x_las[0]);
    book1d(histid1+3,theta_y_las[0]);
    book1d(histid1+4,laspos10_c[0]);
    book1d(histid1+5,laspos10_c[1]);
    book1d(histid1+6,theta_x_las[1]);
    book1d(histid1+7,theta_y_las[1]);
    book1d(histid1+8,feret_las[28]);

    book2d(histid2,laspos[9][0],laspos[9][1]);
    book2d(histid2+1,laspos[9][0],theta_x_las[0]);
    book2d(histid2+2,theta_x_las[0],theta_y_las[0]);
    book2d(histid2+3,laspos[9][0],theta_x_las[1]);
    book2d(histid2+4,laspos10_c[0],theta_x_las[1]);
    book2d(histid2+5,laspos[9][1],theta_y_las[0]);
    book2d(histid2+6,theta_x_las[1],theta_y_las[1]);
    book2d(histid2+7,laspos10_c[1],theta_x_las[1]);

    for(i=0;i<6;i++){
      if(feret_las[4*i]*feret_las[4*i+2]){
	histid1=300+i;
	book2d(histid1+20,laspos[9][0],(float)particleid_las[i]);
	book2d(histid1+30,laspos[9][0],(float)fera_las[2*i]);
	book2d(histid1+40,laspos[9][0],(float)fera_las[2*i+1]);
	if(feret_las[26]>1600&&feret_las[26]<1750)
	  book2d(histid1+60,laspos[9][0],(float)particleid_las[i]);

      }
    }
    for(i=0;i<4;i++){
      if(lasfr.good&(1<<i))book2d(350+i,theta_x_las[1],lasfr.plane[i]);
      if(lasrr.good&(1<<i))book2d(354+i,theta_y_las[1],lasrr.plane[i]);
    }
  }
  
}

/**********************************************************************/

/*** NPOL **************************************/
#ifdef _NPOL_ON
/*if(eventid[POLAR_EVENT]){*/
if(eventid[GR_FP_EVENT]){
  int itmp;
  book1d(200,(float)feret_gr[96]);
  for(i=0;i<27;i++){
    for(j=0;j<2;j++){
      if(feret_npol[i][j]){
	itmp=j*27+i;
	book2d(201,(float)itmp,(float)fera_npol[i][j]);
	book2d(202,(float)itmp,(float)feret_npol[i][j]);
	book2d(205,(float)itmp,(float)t_npol[i][j]);
      }
    }
    if(hnpol[i]){
      book2d(203,(float)i,(float)pid_npol[i]);
      book2d(204,(float)i,(float)pos_npol[i]);
      book2d(206,(float)i,(float)tof_npol[i]);
    }
  }
  for(i=0;i<grssd.k_tdc;i++) {
    short tmpch;
    tmpch=grssd.tdc_ch[i];
    if(tmpch<20) book2d(207,(float)i,grssd.tdc[tmpch]);
    else book2d(207,(float)i,grssd.tdc[tmpch]);
  }
}
#endif

/*** VDC EFF CHECK **************************************/
#ifdef _VDC_EFF_CHECK
#if 0
histid2=600;
if(good_vdc){
  book2d(histid2,thsimp[0][0],theta_x[0]); 
  book2d(histid2+1,thsimp[2][0],theta_x[0]); 
  book1d(histid2+2,fppos[0][0]-vdcpos[9][0]);
  book1d(histid2+3,fppos[2][0]-vdcpos[9][0]);
  book1d(histid2+4,thsimp[0][0]-theta_x[0]);
  book1d(histid2+5,thsimp[2][0]-theta_x[0]);

  book1d(histid2+6,fppos[0][1]-vdcpos[9][1]);
  book1d(histid2+7,fppos[1][1]-vdcpos[9][1]);
  book1d(histid2+8,fppos[2][1]-vdcpos[9][1]);
  book1d(histid2+9,fppos[3][1]-vdcpos[9][1]);

  book1d(histid2+10,exsimp[0]-ex_energy);
  book1d(histid2+11,exsimp[1]-ex_energy);
  book1d(histid2+12,exsimp[2]-ex_energy);
  book1d(histid2+13,exsimp[3]-ex_energy);
}

if(good_pid1){
  if(posids&0x0100) {
    if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+20,exsimp[0],0.0);
    book2d(histid2+20,exsimp[0],4.0);
  }
  if(posids&0x0200){
    book2d(histid2+20,exsimp[1],5.0);
    if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+20,exsimp[1],1.0);
  }
  if(posids&0x1000) {
    book2d(histid2+20,exsimp[2],6.0);
    if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+20,exsimp[2],2.0);
  }
  if(posids&0x2000) {
    book2d(histid2+20,exsimp[3],7.0);
    if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+20,exsimp[3],3.0);
  }
}
#endif
#endif  /* _VDC_EFF_CHECK */
/*** VDC EFF CHECK **************************************/

