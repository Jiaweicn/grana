/* 98/05/01 analyzing_v1.5.c for ana_v1.5 or later */
/* Compile option _FPP_ON enables FPP routine.*/
/* 98/05/06  change MWPC routine */
/* 00/07/05  add compile option _VDC_EFF_CHECK */

if(eventid[GR_FP_EVENT]||eventid[COIN]){
  /** Trig. Scinti. ************************************************/
  pos_scinti=feret_gr[0]-feret_gr[1]-100.;
  pos_scinti2=feret_gr[2]-feret_gr[3];
  particleid=sqrt((double)fera_gr[0]*fera_gr[1]);
  particleid2=sqrt((double)fera_gr[2]*fera_gr[3]);
  if(particleid>grcr.grpid1[0] && particleid<grcr.grpid1[1]) good_pid1=1;
  if(particleid2>P_ID_L2 && particleid2<P_ID_H2) good_pid2=1;
  good_pid=good_pid1*good_pid2;

  /***** MWDC GR **********/
  /** vdcdata.nhit[0..3] .... Number of hit wire in each plane */
  /** vdcdata.cclu[0..3] .... Number of cluster in each plane */
  /** vdcdata.good .... 0x1:X1(Y1),0x2:X2(Y2),0x4:X3(Y3),0x8:X4(Y4) */
  /** vdcdata.plane[0..3] .... Gravity of cluster when cclu=1 */
  /** vdcdata.nlhit[0..3] .... Cluster size when cclu=1 */
  /** vdcdata.clclu[0..3] .... For efficiency calculation */
  mwdcana(&grfr);
  mwdcana(&grrr);

  /*** efficency check *****/
  if(good_pid){ 
  mwdceff(&grfr);
  mwdceff(&grrr);
  /*** efficency check *****/
  }

  /***** VDC ******************************************************/
  vdcana(&grfx,&vdc_fx);/** GR_FR_X ***/
  vdcana(&grfu,&vdc_fu);/** GR_FR_U ***/
  vdcana(&grrx,&vdc_rx);/** GR_RE_X ***/
  vdcana(&grru,&vdc_ru);/** GR_RE_U ***/
  
  /** ray trace (VDC) *******/
  /* VDC_GR_FX 1 */
  /* VDC_GR_FU 2 */
  /* VDC_GR_RX 4 */
  /* VDC_GR_RU 8 */
  
  if(grfx.good!=1) vdc_chamcheck+=1;else posids|=0x0001;
  if(grfu.good!=1) vdc_chamcheck+=2;else posids|=0x0002;
  if(grrx.good!=1) vdc_chamcheck+=4;else posids|=0x0010;
  if(grru.good!=1) vdc_chamcheck+=8;else posids|=0x0020;
  
  if((posids&0x11)==0x11){
    /* deciding equation of x-plane calculated by VDC-data */
    vectsub(grrx.rpos,grfx.rpos,rayvec_vdc_x);
    plane_eq(rayvec_vdc_x,vdc_fx.wiredirec,grfx.rpos,plane_vdc_x);
  }
  
  if((posids&0x22)==0x22){
    /* deciding equation of u-plane calculated by VDC-data */
    vectsub(grru.rpos,grfu.rpos,rayvec_vdc_u);
    plane_eq(rayvec_vdc_u,vdc_fu.wiredirec,grfu.rpos,plane_vdc_u);
  }
  
  if((posids&0x3)==0x3){  /* Check front chamber */
    if(fabs(grfx.rpos[0]-grfu.rpos[0])>36) vdc_rayid|=0x0008;
  }
  
  if((posids&0x30)==0x30){ /* Check rear chamber */
    if(fabs(grrx.rpos[0]-grru.rpos[0])>36) vdc_rayid|=0x0010;
  }
  
  if(vdc_chamcheck!=0){/* if one of chambers did not detect particle correctly */
    vdc_rayid|=1<<(vdc_chamcheck+10);
  }
  else{ /* if all chambers detect particle correctly */
    /* calculation of position on focal plane */
    if(calcpoint(plane_vdc_x,plane_vdc_u,grplane[9],vdcpos[9]))
      vdc_rayid|=0x0004;
    if(!(vdc_rayid&0x1c)){
      good_vdc=1;vdc_rayid|=1;
      calcpoint(plane_vdc_x,plane_vdc_u,grplane[10],vdcpos[10]);
      calcpoint(plane_vdc_x,plane_vdc_u,grplane[0],vdcpos[0]);
      calcpoint(plane_vdc_x,plane_vdc_u,grplane[7],vdcpos[7]);
      
      /* calculation of angles relative to z-axis on F.P. */
      vectsub(vdcpos[10],vdcpos[9],rayvec_vdc);
      theta_x[0]=scangle_x(rayvec_vdc); 
      /* angle in zx-plane relative to z axis on F.P. */
      theta_y[0]=scangle_y(rayvec_vdc);
      /* angle in yz-plane relative to z axis on F.P. */
      
      /**** corrections *******/
#include "correction.c"
    }
  }
  
  
  /*** efficency check *****/
  if(good_pid){
    if((posids&0x32)==0x32)  grfx.eff++;
    if((posids&0x31)==0x31)  grfu.eff++;
    if((posids&0x23)==0x23)  grrx.eff++;
    if((posids&0x13)==0x13)  grru.eff++;
    if(good_vdc) eff_vdc++;
  }
  /*** efficency check *****/


  
  /** various cut ***************************************************/
  
  if(good_vdc) {
     /** for theta_x_cut **/
    if(theta_x[2]<1.0) theta_c_cut=1;
    
    if(theta_x[1]<THETAX_C_L) theta_x_cut=-1;
    if(theta_x[1]>=THETAX_C_L && theta_x[1]<THETAX_C_H) theta_x_cut=1;
    if(theta_x[1]>=THETAX_C_H)  theta_x_cut=-2;
    
    /** for theta_y_cut **/
    if(theta_y[1]<THETAY_C_L) theta_y_cut=-1;
    if(theta_y[1]>=THETAY_C_L && theta_y[1]<THETAY_C_H) theta_y_cut=1;
    if(theta_y[1]>=THETAY_C_H)  theta_y_cut=-2;
    
    /** for y_cut ***/
#if 0
    if(vdcpos10_c[1] < grcr.yp[0]) y_cut=-1;
    if(grcr.yp[0] <= vdcpos10_c[1] && vdcpos10_c[1] < grcr.yp[1]) y_cut=1;
    if(grcr.yp[1] <= vdcpos10_c[1]) y_cut=-2;
    if(grcr.yb[0] <= vdcpos10_c[1] && vdcpos10_c[1] < grcr.yb[1]) y_back=1;
    if(grcr.yb[2] <= vdcpos10_c[1] && vdcpos10_c[1] < grcr.yb[3]) y_back=1;
#endif    
    if(theta_y[1] < grcr.yp[0]) y_cut=-1;
    if(grcr.yp[0] <= theta_y[1] && theta_y[1] < grcr.yp[1]) y_cut=1;
    if(grcr.yp[1] <= theta_y[1]) y_cut=-2;
    if(grcr.yb[0] <= theta_y[1] && theta_y[1] < grcr.yb[1]) y_back=1;
    if(grcr.yb[2] <= theta_y[1] && theta_y[1] < grcr.yb[3]) y_back=1;
    
    /** for rf_cut ***/
    if((fabs(rf_c-grcr.rfc[0])<grcr.rfw)||
       (fabs(rf_c-grcr.rfc[1])<grcr.rfw)) good_rf=1;

    /*******************************************************/
    if(grcr.grang>2.0&&grcr.grang<6.0&&grcr.igrmode==1){
      y_cut=0;y_back=0;
      if(theta_y[1]<-2.0)  y_cut=-1;
      if(theta_y[1]>2.0)  y_cut=-2;
      if(fabs(theta_y[1])<=2.0)  y_cut=1;
      else if(fabs(theta_y[1])<=4) y_back=1;
    }
    /*******************************************************/
  }
}


/*******************************************************/


/********************************************************/
#ifndef _LAS_OFF
if(eventid[LAS_FP_EVENT]||eventid[COIN]){
  /** Trig. Scinti. LAS ************************************************/
  pos_scinti_las[0]=feret_las[0]-feret_las[2];
  pos_scinti_las[1]=feret_las[4]-feret_las[6];
  pos_scinti_las[2]=feret_las[8]-feret_las[10];
  pos_scinti_las[3]=feret_las[12]-feret_las[14];
  pos_scinti_las[4]=feret_las[16]-feret_las[18];
  pos_scinti_las[5]=feret_las[20]-feret_las[22];
  
  particleid_las[0]=sqrt((double)fera_las[0]*fera_las[1]);
  particleid_las[1]=sqrt((double)fera_las[2]*fera_las[3]);
  particleid_las[2]=sqrt((double)fera_las[4]*fera_las[5]);
  particleid_las[3]=sqrt((double)fera_las[6]*fera_las[7]);
  particleid_las[4]=sqrt((double)fera_las[8]*fera_las[9]);
  particleid_las[5]=sqrt((double)fera_las[10]*fera_las[11]);
  
  
  if(feret_las[28]<650 && feret_las[28]>380) rf_cut_las=1;
  if(feret_las[28]<10 && feret_las[28]>1000) rf_cut_las=1;
  if(particleid_las[0]>PID_LASL1 && particleid_las[0]<PID_LASH1){
    good_pid_las|=0x0001;
  }
  if(particleid_las[1]>PID_LASL2 && particleid_las[1]<PID_LASH2){
    good_pid_las|=0x0002;
  }
  if(particleid_las[2]>PID_LASL3 && particleid_las[2]<PID_LASH3){
    good_pid_las|=0x0004;
  }
  

  /***** MWDC LAS **********/
  /** vdcdata.nhit[0..3] .... Number of hit wire in each plane */
  /** vdcdata.cclu[0..3] .... Number of cluster in each plane */
  /** vdcdata.good .... 0x1:X1(Y1),0x2:X2(Y2),0x4:X3(Y3),0x8:X4(Y4) */
  /** vdcdata.plane[0..3] .... Gravity of cluster when cclu=1 */
  /** vdcdata.nlhit[0..3] .... Cluster size when cclu=1 */
  /** vdcdata.clclu[0..3] .... For efficiency calculation */
  mwdcana(&lasfr);
  mwdcana(&lasrr);

  if(good_pid_las){ 
  /*** efficency check *****/
  mwdceff(&lasfr);
  mwdceff(&lasrr);
  /*** efficency check *****/
  /*******/
  }

  /***** LAS VDC ******************************************************/
  vdcana(&lasfu,&las_fu);/** LAS_FR_U ***/
  vdcana(&lasru,&las_ru);/** LAS_RE_U ***/
  
  /** ray trace (VDC) *******/
#ifdef _LAS_VDC_XU
  vdcana(&lasfx,&las_fx);/** LAS_FR_X ***/
  vdcana(&lasrx,&las_rx);/** LAS_RE_X ***/
  if(lasfx.good!=1) las_chamcheck+=1;else las_posids|=0x0001; /* LAS_FX 1 */
  if(lasfu.good!=1) las_chamcheck+=2;else las_posids|=0x0002; /* LAS_FU 2 */
  if(lasrx.good!=1) las_chamcheck+=4;else las_posids|=0x0010; /* LAS_RX 4 */
  if(lasru.good!=1) las_chamcheck+=8;else las_posids|=0x0020; /* LAS_RU 8 */
#else
  vdcana(&lasfv,&las_fv);/** LAS_FR_V ***/
  vdcana(&lasrv,&las_rv);/** LAS_RE_V ***/
  if(lasfu.good!=1) las_chamcheck+=1;else las_posids|=0x0002;   /* LAS_FU 1 */
  if(lasfv.good!=1) las_chamcheck+=2;else las_posids|=0x0004;   /* LAS_FV 2 */
  if(lasru.good!=1) las_chamcheck+=4;else las_posids|=0x0020;   /* LAS_RU 4 */
  if(lasrv.good!=1) las_chamcheck+=8;else las_posids|=0x0040;   /* LAS_RV 8 */
#endif
  
  if((las_posids&0x11)==0x11){
    /* deciding equation of x-plane calculated by VDC-data */
    vectsub(lasrx.rpos,lasfx.rpos,rayvec_las_x);
    plane_eq(rayvec_las_x,las_fx.wiredirec,lasfx.rpos,plane_las_x);
  }
  
  if((las_posids&0x22)==0x22){
    /* deciding equation of u-plane calculated by VDC-data */
    vectsub(lasru.rpos,lasfu.rpos,rayvec_las_u);
    plane_eq(rayvec_las_u,las_fu.wiredirec,lasfu.rpos,plane_las_u);
  }
  
  if((las_posids&0x44)==0x44){
    /* deciding equation of v-plane calculated by VDC-data */
    vectsub(lasrv.rpos,lasfv.rpos,rayvec_las_v);
    plane_eq(rayvec_las_v,las_fv.wiredirec,lasfv.rpos,plane_las_v);
  }

  if(las_chamcheck!=0){
    /* if one of chambers did not detect particle correctly */
    las_rayid|=1<<(las_chamcheck+10);
  }
  else{ /* if all chambers detect particle correctly */
    
    /* calculation of position on focal plane */
#ifdef _LAS_VDC_XU
    if(calcpoint(plane_las_x,plane_las_u,lasplane[9],laspos[9]))
      las_rayid|=0x0004;
    else{
      good_las=1;las_rayid|=1;
      calcpoint(plane_las_x,plane_las_u,lasplane[10],laspos[10]);
      calcpoint(plane_las_x,plane_las_u,lasplane[0],laspos[0]);
      calcpoint(plane_las_x,plane_las_u,lasplane[7],laspos[7]);
    }
#else
    if(calcpoint(plane_las_u,plane_las_v,lasplane[9],laspos[9]))
      las_rayid|=0x0004;
    else{
      good_las=1;las_rayid|=1;
      calcpoint(plane_las_u,plane_las_v,lasplane[10],laspos[10]);
      calcpoint(plane_las_u,plane_las_v,lasplane[0],laspos[0]);
      calcpoint(plane_las_u,plane_las_v,lasplane[7],laspos[7]);
    }
#endif
  }
  
  if(good_las){
    /* calculation of angles relative to z-axis on F.P. */
    vectsub(laspos[10],laspos[9],rayvec_las);
    theta_x_las[0]=scangle_x(rayvec_las); 
    /* angle in zx-plane relative to z axis on F.P. */
    theta_y_las[0]=scangle_y(rayvec_las);
    /* angle in yz-plane relative to z axis on F.P. */
    lasmat(laspos[9],laspos10_c,theta_x_las,theta_y_las);
  }
  

  if(good_pid_las){ 
  /*** efficency check *****/
#ifdef _LAS_VDC_XU
  if((las_posids&0x32)==0x32)  lasfx.eff++;
  if((las_posids&0x31)==0x31)  lasfu.eff++;
  if((las_posids&0x23)==0x23)  lasrx.eff++;
  if((las_posids&0x13)==0x13)  lasru.eff++;
#else
  if((las_posids&0x64)==0x64)  lasfu.eff++;
  if((las_posids&0x62)==0x62)  lasfv.eff++;
  if((las_posids&0x46)==0x46)  lasru.eff++;
  if((las_posids&0x26)==0x26)  lasrv.eff++;
#endif
  if(good_las) eff_las++;
  /*** efficency check *****/
  }

}  
#endif  /* LAS_OFF */
/********************************************************/

#ifdef _FPP_ON
/***** MWPC ******************************************************/
/** MWPC_X1 **/
mwpcana(&pc_x1,&pcpos_x1);
/** MWPC_X2 **/
mwpcana(&pc_x2,&pcpos_x2);
/** MWPC_U3 **/
mwpcana(&pc_u3,&pcpos_u3);
/** MWPC_V3 **/
mwpcana(&pc_v3,&pcpos_v3);
/** MWPC_U4 **/
mwpcana(&pc_u4,&pcpos_u4);
/** MWPC_V4 **/
mwpcana(&pc_v4,&pcpos_v4);

/** ray trace (MWPC) *******/
if(pc_u3.good!=1) pc_chamcheck+=1;   /* MWPC_U3 1 */
if(pc_v3.good!=1) pc_chamcheck+=2;   /* MWPC_V3 2 */
if(pc_u4.good!=1) pc_chamcheck+=4;   /* MWPC_U4 4 */
if(pc_v4.good!=1) pc_chamcheck+=8;   /* MWPC_V4 8 */

if(pc_chamcheck!=0){/* if one of chambers did not detect particle correctly */
  pc_rayid[++pc_rayid[0]]=pc_chamcheck+10;
}
else{ /* if all chambers detect particle correctly */
/* deciding equation of u-plane calculated by MWPC-data */
  vectsub(pc_u4.rposv,pc_u3.rposv,rayvec_pc_u);
  plane_eq(rayvec_pc_u,pcpos_u3.wiredirec,pc_u3.rposv,plane_pc_u);

/* deciding equation of v-plane calculated by MWPC-data */
  vectsub(pc_v4.rposv,pc_v3.rposv,rayvec_pc_v);
  plane_eq(rayvec_pc_v,pcpos_v3.wiredirec,pc_v3.rposv,plane_pc_v);

  calcpoint(plane_pc_u,plane_pc_v,grplane[10],pcpos[10]);
  calcpoint(plane_pc_u,plane_pc_v,grplane[0],pcpos[0]);
  calcpoint(plane_pc_u,plane_pc_v,grplane[7],pcpos[7]);
  calcpoint(plane_pc_u,plane_pc_v,grplane[8],pcpos[8]);

  if(calcpoint(plane_pc_u,plane_pc_v,grplane[9],pcpos[9]))
    pc_rayid[++pc_rayid[0]]=2;
  else{
    good_pc=1;
    vectsub(pcpos[10],pcpos[9],rayvec_pc);
  }

}

/* second level trigger check *******/
if(good_pc && pc_x1.good && pc_x2.good){
  fpp_theta_u=atan((pc_u4.pos[0]-pc_u3.pos[0]
		    -(pc_x1.pos[0]-pc_x2.pos[0])-66.)/fpp_dis)*R_TO_D;
  fpp_theta_v=atan((pc_v4.pos[0]-pc_v3.pos[0]
		    +(pc_x1.pos[0]-pc_x2.pos[0])-58.)/fpp_dis)*R_TO_D;
}


/* efficiency check ********/
if(pc_x1.good) {
  pc_x1.eff++;
}
if(pc_x2.good) {
  pc_x2.eff++;
}
if(pc_x1.good+pc_x2.good==2) {
  eff_x++;
}
if(pc_v3.good+pc_u4.good+pc_v4.good==3) {
  pc_u3.eff++;
}
if(pc_u3.good+pc_u4.good+pc_v4.good==3) {
  pc_v3.eff++;
}
if(pc_u3.good+pc_v3.good+pc_v4.good==3) {
  pc_u4.eff++;
}
if(pc_u3.good+pc_v3.good+pc_u4.good==3) {
  pc_v4.eff++;
}
if(good_pc) {
  eff_pc++;
}

/** FPP event ***************************************************/
if(good_vdc && good_pc) { 
  /* calculating vertex point */
  vertexcalc[0][0]=vectscpro(rayvec_vdc,rayvec_vdc);
  vertexcalc[0][1]=(-1)*vectscpro(rayvec_vdc,rayvec_pc);
  vertexcalc[0][2]=(-1)*vectscpro(vdcpos[9],rayvec_vdc)
	+vectscpro(pcpos[9],rayvec_vdc);
  vertexcalc[1][0]=vertexcalc[0][1];
  vertexcalc[1][1]=vectscpro(rayvec_pc,rayvec_pc);
  vertexcalc[1][2]=vectscpro(vdcpos[9],rayvec_pc)
	-vectscpro(pcpos[9],rayvec_pc);
  if(calcvertex(vertexcalc,vertextmp)) exit(-1);
  vectconb(1.0,vdcpos[9],vertextmp[0],rayvec_vdc,vertexpos[0]);
  vectconb(1.0,pcpos[9],vertextmp[1],rayvec_pc,vertexpos[1]);

  for(i=0;i<3;i++){
    vertexpos[2][i]=(vertexpos[0][i]+vertexpos[1][i])/2;
  }
  vectsub(vertexpos[0],vertexpos[1],vertexpos[3]);
  vertexdis=sqrt(vectscpro(vertexpos[3],vertexpos[3]));

  /* calculating FPP-scatt. angle */
  theta_x[2]=fppangle_x(rayvec_vdc,rayvec_pc);
  theta_y[2]=fppangle_y(rayvec_vdc,rayvec_pc);


/*  fpp_longvec[0]=0;
  fpp_longvec[1]=0;
  fpp_longvec[2]=1;

  fpp_rotate_y(theta_x[0],rayvec_pc,rayvec_pc);
  fpp_rotate_y(theta_x[0],rayvec_vdc,rayvec_vdc);
  fpp_rotate_x(-scangle_y(rayvec_vdc),rayvec_pc,rayvec_pc);
  fpp_rotate_x(-scangle_y(rayvec_vdc),rayvec_vdc,rayvec_vdc);

  fpp_angle_phi=atan2(rayvec_pc[1],rayvec_pc[0])*R_TO_D;
  fpp_angle_theta=vectangle(rayvec_vdc,rayvec_pc);*/

  fpp_sidevec[0]=rayvec_vdc[2];
  fpp_sidevec[1]=0;
  fpp_sidevec[2]=-rayvec_vdc[0];
  vectvecpro(rayvec_vdc,rayvec_pc,fpp_normalvec);
  vectvecpro(fpp_normalvec,rayvec_vdc,fpp_longvec);

  fpp_angle_theta=vectangle(rayvec_vdc,rayvec_pc);  
  fpp_angle_phi=vectangle(fpp_sidevec,fpp_longvec);
  if(fpp_longvec[1]<0) fpp_angle_phi*=(-1);
  fppangpv=fpp_angle_phi-90;
  if(fppangpv<-180) fppangpv+=360;

/*  for(i=0;i<3;i++)
    printf("vdc[%d]=%f \tpc[%d]=%f\n",i,rayvec_vdc[i],i,rayvec_pc[i]);
  printf("\n");*/

  /** fpp angle cut **/
  if(FPP_THETA_L<=fpp_angle_theta && fpp_angle_theta<=FPP_THETA_H){
    if(fabs(fpp_angle_phi)<FPP_PHI_H && fabs(fpp_angle_phi)>FPP_PHI_L)
      good_angh=1;
    if(fabs(fppangpv)<FPP_PHI_H && fabs(fppangpv)>FPP_PHI_L)
      good_angv=1;
    good_angle=good_angh+good_angv;
  }

  /** fpp vertex position cut **/
  if(vertexpos[2][2]>FPP_VER_L && vertexpos[2][2]<FPP_VER_H)
    good_vertex=1;
}

#endif






/*** VDC EFF CHECK **************************************/
#ifdef _VDC_EFF_CHECK 
if(good_pid1){
  if(!(vdc_rayid&0x10)){  
    if((posids&0x32)==0x32) {
      simp_pl(plane_vdc_u,&grrx,&vdc_rx);
      calcfpsimp2(grrx.plane,plane_vdc_u,fppos[0],thsimp[0]);
      corsimp(fppos[0],thsimp[0],&exsimp[0],thcsimp[0]);
      if(fabs(thcsimp[0][0])<1.5) posids|=0x0100;
    }
    if((posids&0x31)==0x31) {
      simp_pl(plane_vdc_x,&grru,&vdc_ru);
      calcfpsimp2(plane_vdc_x,grru.plane,fppos[1],thsimp[1]);
      corsimp(fppos[1],thsimp[1],&exsimp[1],thcsimp[1]);
      if(fabs(thcsimp[1][0])<1.5) posids|=0x0200;
    }
  }
  if(!(vdc_rayid&0x08)){  
    if((posids&0x23)==0x23) {
      simp_pl(plane_vdc_u,&grfx,&vdc_fx);
      calcfpsimp2(grfx.plane,plane_vdc_u,fppos[2],thsimp[2]);
      corsimp(fppos[2],thsimp[2],&exsimp[2],thcsimp[2]);
      if(fabs(thcsimp[2][0])<1.5) posids|=0x1000;
    }
    if((posids&0x13)==0x13) {
      simp_pl(plane_vdc_x,&grfu,&vdc_fu);
      calcfpsimp2(plane_vdc_x,grfu.plane,fppos[3],thsimp[3]);
      corsimp(fppos[3],thsimp[3],&exsimp[3],thcsimp[3]);
      if(fabs(thcsimp[3][0])<1.5) posids|=0x2000;
    }
  }
}

#ifndef _LAS_OFF
if(good_pid_las){
  if(!(las_rayid&0x10)){  
    if((las_posids&0x64)==0x64) {
      simp_pl(plane_las_v,&lasru,&las_ru);
      calclassimp2(lasru.plane,plane_las_v,lasfppos[0],thlsimp[0]);
      lasmat(&lasfppos[0][0],&lasfppos[0][3],thlsimp[0],phlsimp[0]);
      /*      if(fabs(thcsimp[0][0])<1.5) posids|=0x0100;*/
    }
    if((las_posids&0x62)==0x62) {
      simp_pl(plane_las_u,&lasrv,&las_rv);
      calclassimp2(plane_las_u,lasrv.plane,lasfppos[1],thlsimp[1]);
      lasmat(&lasfppos[1][0],&lasfppos[1][3],thlsimp[1],phlsimp[1]);
      /*      if(fabs(thcsimp[1][0])<1.5) posids|=0x0200;*/
    }
  }
  if(!(las_rayid&0x08)){  
    if((las_posids&0x46)==0x46) {
      simp_pl(plane_las_v,&lasfu,&las_fu);
      calclassimp2(lasfu.plane,plane_las_v,lasfppos[2],thlsimp[2]);
      lasmat(&lasfppos[2][0],&lasfppos[2][3],thlsimp[2],phlsimp[2]);
      /*      if(fabs(thcsimp[2][0])<1.5) posids|=0x1000;*/
    }
    if((posids&0x26)==0x26) {
      simp_pl(plane_las_u,&lasfv,&vdc_fv);
      calcfpsimp2(plane_las_u,lasfv.plane,lasfppos[3],thlsimp[3]);
      lasmat(&lasfppos[2][0],&lasfppos[2][3],thlsimp[2],phlsimp[2]);
      /*      if(fabs(thcsimp[3][0])<1.5) posids|=0x2000;*/
    }
  }
}

/*********** E308 special *************/
#if 0
if((las_posids&0x46)==0x46) {
  good_las=1;
  laspos[9][0]=lasfppos[2][0];
  laspos[9][1]=lasfppos[2][1];
  laspos10_c[0]=lasfppos[2][3];
  laspos10_c[1]=lasfppos[2][4];
  theta_x_las[0]=thlsimp[2][0];theta_x_las[1]=thlsimp[2][1];
  theta_y_las[0]=phlsimp[2][0];theta_y_las[1]=phlsimp[2][1];
}
else good_las=0;
#endif
/*********** E308 special *************/

#endif /** _LAS_OFF **/
#endif

/*** MWPC EFF CHECK **************************************/
#ifdef _FPP_ON
#ifdef _MWPC_EFF_CHECK
if(good_vdc && theta_x_cut){
  calcpoint(plane_vdc_x,plane_vdc_u,grplane[2],vdcpos[2]);

  if(pc_u3.good && pc_v3.good){
    pcposids|=0x0001;
    calcpcsimp(&pc_u3, &pc_v3, &pcpos_u3, &pcpos_v3, rayvec_vdc,
	       vdcpos[2], fppangs[0]);
    book1d(650,fppangs[0][0]);
    book1d(651,fppangs[0][1]);
    if(fppangs[0][0]<FPP_THETA_H && fppangs[0][0]>FPP_THETA_L){
      if(fabs(fppangs[0][2])<FPP_PHI_H &&fabs(fppangs[0][2])>FPP_PHI_L){
        int phistid;
	if(fppangs[0][2]<0) phistid=670; /* if L */
	else phistid=671;      /* if R */
	book2d(phistid,ex_energy,0.0);
	book2d(phistid+2,vdcpos[1][0],0.0);
	if(pc_u4.good) {
	  book2d(phistid,ex_energy,4.0);
	  book2d(phistid+2,vdcpos[1][0],4.0);
	}
	if(pc_v4.good) {
	  book2d(phistid,ex_energy,5.0);
	  book2d(phistid+2,vdcpos[1][0],5.0);
	}
      }
      if(good_pc && good_vertex){
	book1d(654,fppangs[0][0]-fpp_angle_theta);
	book1d(655,fppangs[0][1]-fpp_angle_phi);
#if 0
	book2d(660,fppangs[0][0],fpp_angle_theta);
	book2d(662,fppangs[0][1],fpp_angle_phi);
#endif
      }
    }
  }
  if(pc_u4.good && pc_v4.good){
    pcposids|=0x0002;
    calcpcsimp(&pc_u4, &pc_v4, &pcpos_u4, &pcpos_v4, rayvec_vdc,
	       vdcpos[2], fppangs[1]);
    book1d(652,fppangs[1][0]);
    book1d(653,fppangs[1][1]);
    if(fppangs[1][0]<FPP_THETA_H && fppangs[1][0]>FPP_THETA_L){
      if(fabs(fppangs[1][2])<FPP_PHI_H &&fabs(fppangs[1][2])>FPP_PHI_L){
        int phistid;
	if(fppangs[1][2]<0) phistid=670; /* if L */
	else phistid=671;      /* if R */
	book2d(phistid,ex_energy,1.0);
	book2d(phistid+2,vdcpos[1][0],1.0);
	if(pc_u3.good) {
	  book2d(phistid,ex_energy,2.0);
	  book2d(phistid+2,vdcpos[1][0],2.0);
	}
	if(pc_v3.good) {
	  book2d(phistid,ex_energy,3.0);
	  book2d(phistid+2,vdcpos[1][0],3.0);
	}
      }
      if(good_pc){
	book1d(656,fppangs[1][0]-fpp_angle_theta);
	book1d(657,fppangs[1][1]-fpp_angle_phi);
#if 0
	book2d(661,fppangs[1][0],fpp_angle_theta);
	book2d(663,fppangs[1][1],fpp_angle_phi);
#endif
      }
    }
  }
}
/*** MWPC EFF CHECK **************************************/
#endif
#endif


