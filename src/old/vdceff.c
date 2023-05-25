/* vdceff.c ************/
/* 00/07/08 ***********/

#include <math.h>
#include <stdio.h>

#include "ana9.h"
#include "config.h"
#include "vectcalc.h"
#include "kinema.h"

extern unsigned long block;
extern double *grplane[];
extern double m1,m2,m3,m4,K1;
extern struct vdcdata grfx,grfu,grrx,grru;
extern struct chamber vdc_fx,vdc_fu,vdc_rx,vdc_ru;
extern struct runparm grcr;

int checkposid(short *posid,short id){
  int i;
  for(i=0;i<*posid;){
    i++;
    if(*(posid+i)==id) return(1);
  }
  return(0);
}

void calcfpsimp(struct vdcdata *vdcx,struct vdcdata *vdcu,
		struct chamber *chamx,struct chamber *chamu,
		double *fppos,double *theta){
  double rvec_x[3];
  double rvec_u[3];
  double rvec[3];
  double nvec_x[3];
  double nvec_u[3];
  double plane_x[4];
  double plane_u[4];
  double tmppos[3];


  rvec_x[0]=tan(vdcx->rayang*D_TO_R);
  rvec_x[1]=0.0;
  rvec_x[2]=1.0;

  rvec_u[0]=tan(vdcu->rayang*D_TO_R);
  rvec_u[1]=0.0;
  rvec_u[2]=1.0;

  plane_eq(rvec_x,chamx->wiredirec,vdcx->rpos,plane_x);
  plane_eq(rvec_u,chamu->wiredirec,vdcu->rpos,plane_u);
  calcpoint(plane_x,plane_u,grplane[9],fppos);
  calcpoint(plane_x,plane_u,grplane[10],tmppos);
  vectsub(tmppos,fppos,rvec);
  theta[0]=scangle_x(rvec);
  theta[1]=scangle_y(rvec);
}

double corsimp(double *fppos, double *fpang,double *ex,double *scang){
  double K3,ex_energy,p3lab,theta_x_ray;
  double vdcpos[10][3];
  double vdcpos10_c[3];
  double vdcpos10pc[3];
  double theta_x[3],theta_y[3];

  double rf_c;                /** dummy **/
  double feret_gr[40];
  double fera_gr[40];
  int good_pid,i,j,good_vdc;
  double particleid,particleid2;


#ifdef _VDC_EFF_CHECK
  vdcpos[9][0]=fppos[0];vdcpos[9][1]=fppos[1];vdcpos[9][2]=fppos[2];
  theta_x[0]=fpang[0];theta_y[0]=fpang[1];

#include "correction.c"

  *ex=ex_energy;
  scang[0]=theta_x[1];
#endif
  return(vdcpos[1][0]);

}
