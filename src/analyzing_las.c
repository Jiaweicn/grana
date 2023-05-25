/* 98/05/01 analyzing_v1.5.c for ana_v1.5 or later */
/* Compile option _FPP_ON enables FPP routine.*/
/* 98/05/06  change MWPC routine */
/* 05/10/03  Add LAS_FV and LAS_RV, compile option _LAS_VDC_XU. */


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
/*if(particleid>P_ID_L && particleid<P_ID_H) good_pid=1;*/
/*if(feret_las[28]<660 && feret_las[28]>500) rf_cut_las=1;*/
if(feret_las[28]<730 && feret_las[28]>500) rf_cut_las=1;
if(feret_las[28]<1330 && feret_las[28]>1100) rf_cut_las=1;
if(particleid_las[0]>PID_LASL1 && particleid_las[0]<PID_LASH1){
  good_pid_las|=0x0001;
}
if(particleid_las[1]>PID_LASL2 && particleid_las[1]<PID_LASH2){
  good_pid_las|=0x0002;
}
if(particleid_las[2]>PID_LASL3 && particleid_las[2]<PID_LASH3){
  good_pid_las|=0x0004;
}

/***** LAS VDC ******************************************************/


vdcana(&lasfu,&las_fu);/** LAS_FR_U ***/
vdcana(&lasru,&las_ru);/** LAS_RE_U ***/

vdcana(&lasfx,&las_fx);/** LAS_FR_X ***/
vdcana(&lasrx,&las_rx);/** LAS_RE_X ***/

vdcana(&lasfv,&las_fv);/** LAS_FR_V ***/
vdcana(&lasrv,&las_rv);/** LAS_RE_V ***/


/** ray trace (VDC) *******/
#ifdef _LAS_VDC_XU
if(lasfx.good!=1) las_chamcheck+=1;else las_posids|=0x0001; /* LAS_FX 1 */
if(lasfu.good!=1) las_chamcheck+=2;else las_posids|=0x0002; /* LAS_FU 2 */
if(lasrx.good!=1) las_chamcheck+=4;else las_posids|=0x0010; /* LAS_RX 4 */
if(lasru.good!=1) las_chamcheck+=8;else las_posids|=0x0020; /* LAS_RU 8 */
#else
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

if((las_posids&0x11)==0x22){
  /* deciding equation of u-plane calculated by VDC-data */
  vectsub(lasru.rpos,lasfu.rpos,rayvec_las_u);
  plane_eq(rayvec_las_u,las_fu.wiredirec,lasfu.rpos,plane_las_u);
}

if((las_posids&0x11)==0x44){
  /* deciding equation of v-plane calculated by VDC-data */
  vectsub(lasrv.rpos,lasfv.rpos,rayvec_las_v);
  plane_eq(rayvec_las_v,las_fv.wiredirec,lasfv.rpos,plane_las_v);
}

if(las_chamcheck!=0){/* if one of chambers did not detect particle correctly */
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
  /* run1246.hb */
  laspsca[0]=laspos10_c[0]+theta_x_las[1]*96.772;
  
#if 0
  /**** corrections *******/
#include "correction.c"
  /**** corrections *******/ 
#endif
}



/*** efficency check *****/
#ifdef _LAS_VDC_XU
if((posids&0x32)==0x32)  lasfx.eff++;
if((posids&0x31)==0x31)  lasfu.eff++;
if((posids&0x23)==0x23)  lasrx.eff++;
if((posids&0x13)==0x13)  lasru.eff++;
#else
if((posids&0x64)==0x64)  lasfu.eff++;
if((posids&0x62)==0x62)  lasfv.eff++;
if((posids&0x46)==0x46)  lasru.eff++;
if((posids&0x26)==0x26)  lasrv.eff++;

#endif

if(good_las) {
  eff_las++;
}


