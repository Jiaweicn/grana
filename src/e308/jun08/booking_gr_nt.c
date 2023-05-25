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

/** 3351 ***********************************************************/
histid1=400;
#if 0
for(i=0;i<4;i++){
  book1(392+i,(float)issdmul[i]);
  book1(396+i,(float)issdhitr[i]);
}
#endif
 
for(i=0;i<grssd.k_adc;i++){
  j=grssd.adcc_ch[i];
  /*  if(grssd.tdcc[j]!=0){*/
  histid2=412;
  book2d(histid2,(float)j,(float)grssd.adc[i]);
  book2d(histid2+1,(float)j,(float)grssd.adcc[j]);
  book1(800+j,(float)grssd.adcc[j]);

  if(1){
    if(j<20) histid2=histid1+j/5;
    if(j>=20&&j<24) histid2=404;
    if(j>=24&&j<28) histid2=405;
    if(j>=28&&j<32) histid2=406;
    if(j!=29) book2d(histid2,(float)grssd.adcc[j],(float)(grssd.tdcc[j]));
  }
}

for(i=0;i<4;i++){
  if(issdhitr[i]==7||issdhitr[i]==15) {
    book2d(407,grssd.adcc[ihit3[i]],grssd.adcc[24+i]);
    if(issdec[i]) book2d(412+i,grssd.adcc[ihit3[i]],ene_ssdr[i]);    
    else book2d(412+i,grssd.adcc[ihit3[i]],ene_ssd[i]);
  }

  if(issdhitr[i]==15) book2d(408+i,grssd.adcc[24+i],grssd.adcc[28+i]);
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
/*
for(channel=0;channel<32;channel++){
  if(feret_gr[channel]!=0){
    book1(histid1+channel,(float)feret_gr[channel]);}
}*/
for(channel=0;channel<5;channel++){
    book1(histid1+channel,(float)feret_gr[channel]);
}


/** Trig. Scinti. ***************************************************/
if(!eventid[LAS_FP_EVENT] && !eventid[BLOCK_END]){
  if(good_pid1){
	book1(180,(float)pos_scinti);
	book1(181,(float)pos_scinti2);
  }
  

  if(good_vdc){
	book1d(182,(float)particleid);
	book1d(183,(float)particleid2);
	/*	book2d(322,(float)particleid,(float)particleid2);*/
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
if(good_vdc==1){

/******* GR_FP_EVENT ***********/
#if 0
  if(eventid[GR_FP_EVENT]&&good_rf){
    if(good_pid){
      int tmpid=700;
      cmtrk_.xyab[0]=(float)vdcpos[9][0];
      cmtrk_.xyab[1]=(float)vdcpos[9][1];
      cmtrk_.xyab[2]=(float)theta_x[0];
      cmtrk_.xyab[3]=(float)theta_y[0];
      cmtrk_.xyab[4]=(float)vdcpos10_c[0];
      cmtrk_.xyab[5]=(float)vdcpos10_c[1];
      cmtrk_.xyab[6]=(float)theta_x[1];
      cmtrk_.xyab[7]=(float)theta_y[1];
      hfnt_(&tmpid);
    }
  }
#endif
/******* GR_FP_EVENT ***********/

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
    book2d(histid2+3,vdcpos[9][0],theta_x[1]);
    book2d(histid2+4,vdcpos10_c[0],theta_x[1]);
    
    
    book2d(histid2+5,vdcpos10_c[0],vdcpos10_c[1]);
    if(theta_x_cut==1) book2d(histid2+6,vdcpos10_c[0],theta_y[1]);
    book2d(histid2+7,vdcpos10_c[1],theta_y[1]);
    book2d(histid2+8,theta_x[1],theta_y[1]);
    if(fabs(ex_energy)<1.5){
      book2d(330,theta_x[1],theta_y[1]);
      book2d(331,theta_x[0],vdcpos10_c[1]);
    }
    if(ex_energy>2.5&&ex_energy<4){
      book1d(152,ex_energy);
    }
    if(vdcpos10_c[0]>-2.1&&vdcpos10_c[0]<-1.5){
      book1d(150,vdcpos10_c[0]);
      book1d(151,vdcpos10_c[1]);
    }
  }
  if(good_rf){
    book2d(histid2+9,vdcpos[9][0],rf_c);
    book2d(histid2+10,theta_x[1],rf_c);
  }

  if(theta_x_cut==1) {

    book1d(histid1+10,vdcpos10_c[0]);
    book1d(histid1+11,vdcpos10_c[1]);
    if(fabs(theta_x[1])<0.4) book2d(histid2+11,vdcpos10_c[0],theta_y[1]);
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

  book2d(320,vdcpos10_c[0],theta_x[2]);
  book2d(321,ex_energy,theta_x[2]);
  book2d(322,ex_energy,theta_x[2]);

  /*** Recoil Counter Gate ***/
#if 0
  if(y_cut==1){
	if(vdcpos10_c[0]>0.29 && vdcpos10_c[0]<1.53){
	  book1d(histid1+10,theta_x[1]);	
	}
	if(vdcpos10_c[0]>0.29 && vdcpos10_c[0]<1.53){
	  if(good_rec1) book1d(histid1+11,theta_x[1]);
	  if(back_rec1)	book1d(histid1+12,theta_x[1]);
	}
	if(good_rec1) book2d(histid2+20,vdcpos10_c[0],theta_x[1]);  
	else book2d(histid2+21,vdcpos10_c[0],theta_x[1]);
	if(back_rec1) book2d(histid2+22,vdcpos10_c[0],theta_x[1]);  
  }
#endif
  
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
      for(i=0;i<4;i++){
	if(issdhitr[i]==3||issdhitr[i]==7||issdhitr[i]==15){
	  book2d(416+i,ex_energy,ene_ssd[i]);
	  if(issdhitr[i]==3) {
	    book2d(424,ex_energy,ene_ssd[i]);
	    book2d(428,ex_energy,ex_energy-ene_ssd[i]*6.0/4.7);
	  }
	  else{
	    double tmpene;
	    if(issdec[i]) tmpene=ene_ssdr[i];
	    else tmpene=ene_ssd[i];
	    switch(issdpid[i]){
	    case 1:
	      book2d(425,ex_energy,tmpene);
	      book2d(429,ex_energy,ex_energy-tmpene*5.0/4.7*24.0/23.0);
	      break;
	    case 2:
	      book2d(426,ex_energy,tmpene);
	      break;
	    case 3:
	      book2d(427,ex_energy,tmpene);
	      break;
	    }
	  }
	}
      }
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
	  if(fabs(theta_y[1])>1.5) book1d(502,ex_energy);
        }
      }
      else actheta=theta_x[1];          
      book2d(500,ex_energy,actheta);
      /*      book2d(318,ex_energy,theta_x[0]);*/
      /*      book2d(333,vdcpos10_c[1],(double)block);*/
      if(grcr.grang==2.3){
	if(theta_x[1]>-0.8 && theta_x[1]<-0.5811 && fabs(theta_y[1])<0.8){
	  book1d(502,ex_energy);
	}
      }
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
      for(i=0;i<4;i++){
	if(issdhitr[i]==3||issdhitr[i]==7||issdhitr[i]==15){
	  book2d(420+i,ex_energy,ene_ssd[i]);
	  if(issdhitr[i]==3) book2d(432,ex_energy,ene_ssd[i]);
	  else{
	    double tmpene;
	    if(issdec[i]) tmpene=ene_ssdr[i];
	    else tmpene=ene_ssd[i];
	    switch(issdpid[i]){
	    case 1:
	      book2d(433,ex_energy,tmpene);
	      break;
	    case 2:
	      book2d(434,ex_energy,tmpene);
	      break;
	    case 3:
	      book2d(435,ex_energy,tmpene);
	      break;
	    }
	  }
	}
      }
      if(grcr.grang==0.0){
	if(fabs(theta_x[1])<0.8){
	  if(fabs(theta_y[1])<3.3){
	    if(fabs(theta_x[1])<0.4 && fabs(theta_y[1])<2.9){
	      book2d(501,ex_energy,-0.6);
	    }
	    else book2d(501,ex_energy,-0.2);
	  }
	  if(fabs(theta_y[1])<4.2189) book2d(501,ex_energy,0.2);
#if 0
	  if(fabs(theta_y[1])<2.9605) book2d(501,ex_energy,0.2);
	  if(fabs(theta_y[1])<3.7395) book2d(501,ex_energy,0.6);
#endif
	  if(fabs(theta_y[1])<2.7189) book1d(503,ex_energy);
        }
      }
      else book2d(501,ex_energy,theta_x[1]);
      if(grcr.grang==2.3){
	if(theta_x[1]>-0.8 && theta_x[1]<-0.5811 && fabs(theta_y[1])<2.8){
	  book1d(503,ex_energy);
	}
      }
      /*      book2d(319,ex_energy,theta_x[0]);*/
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





