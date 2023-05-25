/* mwpceff.c *******************/
/* 00/07/12  *******************/

#include <math.h>
#include <stdio.h>

#include "ana10.h"
#include "config.h"
#include "vectcalc.h"

extern double *grplane[];

void calcpcsimp(struct mwpcdata *mwpcu, struct mwpcdata *mwpcv,
		struct chamber *chamu, struct chamber *chamv,
		double *rayvdc, double *vertex,double *ang){
  double rvec_u[3];
  double rvec_v[3];
  double rvec[3];
  double nvec_u[3];
  double nvec_v[3];
  double plane_u[4];
  double plane_v[4];
  double tmppos[3];

  double sidevec[3];
  double normvec[3];
  double longvec[3];

  vectsub(mwpcu->rposv,vertex,rvec_u);
  plane_eq(rvec_u,chamu->wiredirec,vertex,plane_u);

  vectsub(mwpcv->rposv,vertex,rvec_v);
  plane_eq(rvec_v,chamv->wiredirec,vertex,plane_v);

  calcpoint(plane_u,plane_v,grplane[7],tmppos);
  vectsub(tmppos,vertex,rvec);
  
  sidevec[0]=*(rayvdc+2);
  sidevec[1]=0.0;
  sidevec[2]=*rayvdc*(-1.0);

  vectvecpro(rayvdc,rvec,normvec);
  vectvecpro(normvec,rvec,longvec);

  ang[0]=vectangle(rayvdc,rvec);
  ang[1]=vectangle(sidevec,longvec);
  if(longvec[1]<0) ang[1]*=(-1);
  ang[2]=ang[1]-90;
  if(ang[2]<-180) ang[2]+=360;
}


