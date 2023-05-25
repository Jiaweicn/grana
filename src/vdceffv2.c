/* vdceffv2.c ************/
/* 07/01/17 ***********/

#include <math.h>
#include <stdio.h>

#include "ana10.h"
#include "config.h"
#include "vectcalc.h"
#include "kinema.h"

extern unsigned long block;
extern double *grplane[];
extern double *lasplane[];
extern double m1,m2,m3,m4,K1;
extern struct vdcdata grfx,grfu,grrx,grru;
extern struct chamber vdc_fx,vdc_fu,vdc_rx,vdc_ru;
extern struct runparm grcr;

void simp_pl(double *fixplane,struct vdcdata *vdc,struct chamber *cham){
  static double ey[3]={0,1,0};
  double tmpvec[3];
  double tmpray[3];

  vectvecpro(fixplane,ey,tmpvec);
  plane_eq(tmpvec,cham->wiredirec,vdc->rpos,vdc->plane);
}

void calcfpsimp2(double *planex,double *planeu,double *fppos,double *theta){
  double tmppos[3];
  double tmpray[3];

  calcpoint(planex,planeu,grplane[9],fppos);
  calcpoint(planex,planeu,grplane[10],tmppos);
  vectsub(tmppos,fppos,tmpray);
  theta[0]=scangle_x(tmpray);
  theta[1]=scangle_y(tmpray);
}

void calclassimp2(double *planex,double *planeu,double *fppos,double *theta){
  double tmppos[3];
  double tmpray[3];

  calcpoint(planex,planeu,lasplane[9],fppos);
  calcpoint(planex,planeu,lasplane[10],tmppos);
  vectsub(tmppos,fppos,tmpray);
  theta[0]=scangle_x(tmpray);
  theta[1]=scangle_y(tmpray);
}

double corsimp(double *fppos, double *fpang,double *ex,double *scang){
  double K3,ex_energy,p3lab,theta_x_ray;
  double vdcpos[10][3];
  double vdcpos10_c[3];
  double vdcpos10pc[3];
  double theta_x[3],theta_y[3];

  double rf_c;                /** dummy **/
  double rf_ct,rf_t,rfaz[2];                /** dummy **/
  double feret_gr[40];
  double fera_gr[40];
  int good_pid,i,j,good_vdc;
  double particleid,particleid2;

#ifdef _VDC_EFF_CHECK
  vdcpos[9][0]=fppos[0];vdcpos[9][1]=fppos[1];vdcpos[9][2]=fppos[2];
  theta_x[0]=fpang[0];theta_y[0]=fpang[1];

#include "correction.c"

  *ex=ex_energy;
  scang[0]=theta_x[1];scang[1]=theta_y[1];scang[2]=theta_x[2];
  fppos[3]=vdcpos10_c[0];fppos[4]=vdcpos10_c[1];fppos[5]=vdcpos10_c[2];

#endif
  return(vdcpos[1][0]);

}
