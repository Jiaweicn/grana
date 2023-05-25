/** booking_v1.6_nt.c 05/10/28 for histdef_nt.f ******************/
/** e253 analysis with ntuple ************/

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
/*for(channel=0;channel<32;channel++){
  if(fera_gr[channel]!=0){
    book1(histid1+channel,(float)fera_gr[channel]);}
}
*/
for(channel=0;channel<4;channel++){
  book1(histid1+channel,(float)fera_gr[channel]);
}

/** FERET ***********************************************************/
histid1=14;


for(channel=0;channel<5;channel++){
    book1(histid1+channel,(float)feret_gr[channel]);
}


/** Trig. Scinti. ***************************************************/
if(!eventid[LAS_FP_EVENT] && !eventid[BLOCK_END]){
  /*  if(good_pid1){*/
	book1(180,(float)pos_scinti);
	book1(181,(float)pos_scinti2);
	/*  }*/
  

  if(good_vdc){
	book1d(182,(float)particleid);
	book1d(183,(float)particleid2);
	/*  if(good_pid)  {*/
	if(good_rf)  {
	  book1d(184,(float)particleid);
	  book1d(185,(float)particleid2);
	}
  }
}

/** 3377VDC *********************************************************/
histid1=90;
/******* GR_FP_EVENT ***********/
if(eventid[GR_FP_EVENT]&&good_pid1){
/*if(1){*/
/** GR_FR_X **/
histid2=histid1;
for(k=0;k<grfx.wire;k++) {  
  book1(histid2,(float)grfx.channel[k]);
//  book1(histid2+4,(float)grfx.time[k]);
// added by Xiaodong Tang o check drifting time
  if(vdcpos[9][0]>0) book1(histid2+4,(float)grfx.time[k]);
}

j=19;
for(k=0;k<grfx.nclu;k++){
  if(grfx.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grfx.nhit[k]);
}

j=19;
if(grfx.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grfx.nclu);

for(k=0;k<grfx.posid;k++){
  if(grfx.posid&(1<<k)) book1(histid2+16,(float)k);
}

#if 0
printf("nclu:%d nlclu:%d posid:",grfx.nclu,grfx.nlclu);
for(k=1;k<grfx.posid[0]+1;k++)printf("%2d ",grfx.posid[k]);
printf("\n");
for(k=0;k<grfx.wire;k++) {  
  printf("%s:%4d %4d\n",grfx.label,grfx.channel[k],grfx.time[k]);
}
printf("\n");
#endif

for(k=0;k<grfx.wire;k++) {
#if 0
  book1d(histid2+20,grfx.table[grfx.time[k]]);
#endif

  if(grfx.time[k]<=grfx.time_h&&grfx.time[k]>=grfx.time_l){
#if 0
    double tmpa,tmpb,tmpr;
    tmpb=drand48();tmpa=1.0-tmpb;
    
    tmpr=grfx.table[grfx.time[k]]*tmpa
      +grfx.table[grfx.time[k]+1]*tmpb
      +(grfx.derr[grfx.time[k]]*tmpa*(tmpa*tmpa-1.0)
	+grfx.derr[grfx.time[k]+1]*tmpb*(tmpb*tmpb-1.0))/6.0;

    tmpr=grfx.table[grfx.time[k]]*tmpa
      +grfx.table[grfx.time[k]+1]*tmpb;
#endif

    book1d(histid2+20,t2dl(&grfx,k,1));
  }
}

if(grfx.good==1){
  book1d(histid2+24,grfx.pos[0]);
  /*if(grfx.posid&0x0200)book1d(histid2+124,grfx.pos[0]);*/
  book1d(histid2+28,grfx.dis);
  /*  if(grfx.posid&0x0100)  book2d(histid2+32,grfx.res[0],grfx.res[1]);*/
}
     
/** GR_FR_U **/
histid2=histid1+1;
for(k=0;k<grfu.wire;k++) {  
  book1(histid2,(float)grfu.channel[k]);
//  book1(histid2+4,(float)grfu.time[k]);
// added by Xiaodong Tang o check drifting time
  if(vdcpos[9][0]>0) book1(histid2+4,(float)grfu.time[k]);
}

j=19;
for(k=0;k<grfu.nclu;k++){
  if(grfu.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grfu.nhit[k]);
}

j=19;
if(grfu.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grfu.nclu);

for(k=0;k<grfu.posid;k++){
  if(grfu.posid&(1<<k)) book1(histid2+16,(float)k);
}

     
for(k=0;k<grfu.wire;k++) {
  book1d(histid2+20,t2dl(&grfu,k,1));
}

if(grfu.good==1){
  book1d(histid2+24,grfu.pos[0]);
  book1d(histid2+28,grfu.dis);
  /*  if(grfu.posid&0x0100) book2d(histid2+32,grfu.res[0],grfu.res[1]);*/
}

/** GR_RE_X **/
histid2=histid1+2;
for(k=0;k<grrx.wire;k++) {  
  book1(histid2,(float)grrx.channel[k]);
  // added by Xiaodong Tang o check drifting time
  if(vdcpos[9][0]>0) book1(histid2+4,(float)grrx.time[k]);
}

j=19;
for(k=0;k<grrx.nclu;k++){
  if(grrx.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grrx.nhit[k]);
}

j=19;
if(grrx.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grrx.nclu);

for(k=0;k<grrx.posid;k++){
  if(grrx.posid&(1<<k)) book1(histid2+16,(float)k);
}

for(k=0;k<grrx.wire;k++) {
  book1d(histid2+20,t2dl(&grrx,k,1));
}

if(grrx.good==1){
  book1d(histid2+24,grrx.pos[0]);
  book1d(histid2+28,grrx.dis);
  /*  if(grrx.posid&0x0100) book2d(histid2+32,grrx.res[0],grrx.res[1]);*/
}

/** GR_RE_U **/     
histid2=histid1+3;
for(k=0;k<grru.wire;k++) {  
  book1(histid2,(float)grru.channel[k]);
//  book1(histid2+4,(float)grru.time[k]);
  // added by Xiaodong Tang o check drifting time
  if(vdcpos[9][0]>0) book1(histid2+4,(float)grru.time[k]);
}

j=19;
for(k=0;k<grru.nclu;k++){
  if(grru.nhit[k]>19) book1(histid2+8,(float)j);
  else book1(histid2+8,(float)grru.nhit[k]);
}

j=19;
if(grru.nclu>19) book1(histid2+12,(float)j);
else book1(histid2+12,(float)grru.nclu);

for(k=0;k<grru.posid;k++){
  if(grru.posid&(1<<k))book1(histid2+16,(float)k);
}

for(k=0;k<grru.wire;k++) {
  book1d(histid2+20,t2dl(&grru,k,1));
}

if(grru.good==1){
  book1d(histid2+24,grru.pos[0]);
  book1d(histid2+28,grru.dis);
  /*  if(grru.posid&0x0100) book2d(histid2+32,grru.res[0],grru.res[1]);*/
}

for(k=0;k<26;k++){
  if(vdc_rayid&(1<<k)) book1(122,(float)k);
}
 }
/******* GR_FP_EVENT ***********/

/*******************************************************************/


histid1=130;histid2=300;


/******* GR_FP_EVENT ***********/
if((flag & 0x8)==0){
  int tmpid=700;
  if(good_vdc==1){
    cmtrk_.xyab[0]=(float)vdcpos[9][0];
    cmtrk_.xyab[1]=(float)vdcpos[9][1];
    cmtrk_.xyab[2]=(float)theta_x[0];
    cmtrk_.xyab[3]=(float)theta_y[0];
  }
  else{
    cmtrk_.xyab[0]=-10000;
    cmtrk_.xyab[1]=-10000;
    cmtrk_.xyab[2]=-10000;
    cmtrk_.xyab[3]=-10000;
  }
  cmtrk_.xyab[4]=(float)fera_gr[0];
  cmtrk_.xyab[5]=(float)fera_gr[1];
  cmtrk_.xyab[6]=(float)fera_gr[2];
  cmtrk_.xyab[7]=(float)fera_gr[3];
  cmtrk_.xyab[8]=(float)feret_gr[0];
  cmtrk_.xyab[9]=(float)feret_gr[1];
  cmtrk_.xyab[10]=(float)feret_gr[2];
  cmtrk_.xyab[11]=(float)feret_gr[3];
  cmtrk_.xyab[12]=(float)feret_gr[4];
//  hfnt_(&tmpid);
  root_tree->Fill();
 }
/******* GR_FP_EVENT ***********/


if(good_vdc==1){

  book1d(histid1,vdcpos[9][0]);
  book1d(histid1+1,vdcpos[9][1]);
  book1d(histid1+2,theta_x[0]);
  book1d(histid1+3,theta_y[0]);
  book1d(histid1+4,vdcpos10_c[0]);
  book1d(histid1+5,vdcpos10_c[1]);
  book1d(histid1+6,theta_x[1]);
  book1d(histid1+7,theta_y[1]);
  book1d(histid1+8,rf_c);
  if(good_rf)book1d(histid1+9,rf_c);  

  if(good_rf){
    book2d(histid2,vdcpos[9][0],vdcpos[9][1]);
    book2d(histid2+1,vdcpos[9][0],theta_x[0]);
    book2d(histid2+2,theta_x[0],theta_y[0]);
    book2d(histid2+3,vdcpos[9][1],theta_y[0]);
    book2d(histid2+4,vdcpos10_c[0],theta_x[1]);
    book2d(histid2+5,vdcpos10_c[0],vdcpos10_c[1]);
    book2d(histid2+6,vdcpos10_c[0],theta_y[1]);
    book2d(histid2+7,theta_x[1],theta_y[1]);


  }
  if(good_rf&&y_cut==1){
    book2d(histid2+9,vdcpos[9][0],rf_c);
    book2d(histid2+10,theta_x[0],rf_c);
  }

  if(theta_x_cut==1) {

    book1d(histid1+10,vdcpos10_c[0]);
    book1d(histid1+11,vdcpos10_c[1]);

    book2d(histid2+12,vdcpos10_c[0],fera_gr[0]);
    book2d(histid2+13,vdcpos10_c[0],fera_gr[1]);
    book2d(histid2+14,vdcpos10_c[0],particleid);
    book2d(histid2+15,vdcpos10_c[0],fera_gr[2]);
    book2d(histid2+16,vdcpos10_c[0],fera_gr[3]);
    book2d(histid2+17,vdcpos10_c[0],particleid2);
  }
}



/* PID & GOODPID *********/
if(good_vdc==1 && good_rf){
  histid1=140;histid2=310;

  
  /* THETA and PHI cut *********/  
  if(theta_x_cut==1){
    double actheta=-100;
  /* y-cut ***********/
    switch(y_cut) {
    case -1:
      book1d(histid1+2,vdcpos10_c[0]);
      book1d(histid1+3,vdcpos10_c[1]);
      break;
    case 1:
      book1d(histid1+4,vdcpos10_c[0]);
      book1d(histid1+5,vdcpos10_c[1]);
      book1d(5,ex_energy);
      if(grcr.grang==0.0){
	if(fabs(theta_x[1])<0.8){
	  if((fabs(theta_x[1])<0.4)&&(fabs(theta_y[1])<0.4)) actheta=-0.6;
	  else {
	    if(fabs(theta_y[1])<0.8) actheta=-0.2;
            else {
#if 0
              if(fabs(theta_y[1])<1.2605) actheta=0.2;
              else actheta=0.6;
#endif
	      actheta=0.2;
            }
	  }
        }
      }
      else actheta=theta_x[1];          
      book2d(500,ex_energy,actheta);
      book2d(334,ex_energy,(double)block);
      break;
    case -2:
      book1d(histid1+6,vdcpos10_c[0]);
      book1d(histid1+7,vdcpos10_c[1]);
      break;
    }
    if(y_back){
      book1d(histid1+8,vdcpos10_c[0]);
      book1d(histid1+9,vdcpos10_c[1]);
      book1d(6,ex_energy);
      if(grcr.grang==0.0){
	if(fabs(theta_x[1])<0.8){
	  if(fabs(theta_y[1])<3.3){
	    if(fabs(theta_x[1])<0.4 && fabs(theta_y[1])<2.9){
	      book2d(501,ex_energy,-0.6);
	    }
	    else book2d(501,ex_energy,-0.2);
	  }
	  if(fabs(theta_y[1])<4.2189) book2d(501,ex_energy,0.2);
        }
      }
      else book2d(501,ex_energy,theta_x[1]);
    }
  }
  /* THETA and PHI cut *********/  
}
/* PID & GOODPID *********/


/*** VDC EFF CHECK **************************************/

#ifdef _VDC_EFF_CHECK
if(good_pid1) {

  histid2=600;
  if(good_vdc){
    book2d(histid2,thsimp[0][0],theta_x[0]); 
    book2d(histid2+1,thsimp[1][0],theta_x[0]); 
    book2d(histid2+2,thsimp[2][0],theta_x[0]); 
    book2d(histid2+3,thsimp[3][0],theta_x[0]); 
    
    book2d(histid2+4,fppos[0][0],vdcpos[9][0]); 
    book2d(histid2+5,fppos[1][0],vdcpos[9][0]); 
    book2d(histid2+6,fppos[2][0],vdcpos[9][0]); 
    book2d(histid2+7,fppos[3][0],vdcpos[9][0]); 
    
    book1d(histid2+8,fppos[0][0]-vdcpos[9][0]);
    book1d(histid2+9,fppos[2][0]-vdcpos[9][0]);
    book1d(histid2+10,thcsimp[0][0]-theta_x[1]);
    book1d(histid2+11,thcsimp[2][0]-theta_x[1]);

    book1d(histid2+12,fppos[0][1]-vdcpos[9][1]);
    book1d(histid2+13,fppos[1][1]-vdcpos[9][1]);
    book1d(histid2+14,fppos[2][1]-vdcpos[9][1]);
    book1d(histid2+15,fppos[3][1]-vdcpos[9][1]);

    book1d(histid2+16,exsimp[0]-ex_energy);
    book1d(histid2+17,exsimp[1]-ex_energy);
    book1d(histid2+18,exsimp[2]-ex_energy);
    book1d(histid2+19,exsimp[3]-ex_energy);

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

    if(posids&0x0100) {
      if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+30,exsimp[0],0.0);
      book2d(histid2+30,exsimp[0],4.0);
    }
    if(posids&0x0200){
      book2d(histid2+30,exsimp[1],5.0);
      if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+30,exsimp[1],1.0);
    }
    if(posids&0x1000) {
      book2d(histid2+30,exsimp[2],6.0);
      if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+30,exsimp[2],2.0);
    }
    if(posids&0x2000) {
      book2d(histid2+30,exsimp[3],7.0);
      if(good_vdc && fabs(theta_x[1])<1.5) book2d(histid2+30,exsimp[3],3.0);
    }

  }

}
#endif
/*** VDC EFF CHECK **************************************/





