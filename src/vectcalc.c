#include <math.h>
#include "ana10.h"

void vectadd(double *u, double *v, double *x){
  *x=*u+*v;
  *(x+1)=*(u+1)+*(v+1);
  *(x+2)=*(u+2)+*(v+2);
}

void vectsub(double *u, double *v, double *x){
  *x=*u-*v;
  *(x+1)=*(u+1)-*(v+1);
  *(x+2)=*(u+2)-*(v+2);
}

void vectlin(double a, double *u, double *x){
  *x=(*u)*a;
  *(x+1)=(*(u+1))*a;
  *(x+2)=(*(u+2))*a;
}

void vectconb(double a, double *u, double b, double *v, double *x){
  double tmp1[3],tmp2[3];
  vectlin(a,u,tmp1);
  vectlin(b,v,tmp2);
  vectadd(tmp1,tmp2,x);
}

double vectscpro(double *u,double *v){
  double x;
  x=(*u)*(*v)+(*(u+1))*(*(v+1))+(*(u+2))*(*(v+2));
  return(x);
}

void vectunit(double *u){
  double x;
  x=sqrt(vectscpro(u,u));
  *u/=x;
  *(u+1)/=x;
  *(u+2)/=x;
}

void vectvecpro(double *u,double *v,double *x){
  *x=(*(u+1))*(*(v+2))-(*(u+2))*(*(v+1));
  *(x+1)=(*(u+2))*(*v)-(*u)*(*(v+2));
  *(x+2)=(*u)*(*(v+1))-(*(u+1))*(*v);
}

double vectangle(double *u, double *v){
  double a;
  a=vectscpro(u,v)/sqrt(vectscpro(u,u)*vectscpro(v,v));
  return(acos(a)*R_TO_D);
}

void vectprojx(double *u, double *x){
  *x=*u;
  *(x+1)=0;
  *(x+2)=*(u+2);
}

void vectprojy(double *u, double *x){
  *x=0;
  *(x+1)=*(u+1);
  *(x+2)=*(u+2);
}

double scangle_x(double *u){
  double tmp1[3],tmp2[3]={0,0,1.},a;
  vectprojx(u,tmp1);
  a=vectangle(tmp1,tmp2);
  if(tmp1[0]<0) return(-1*a);
  return(a);
}

double scangle_y(double *u) {
  double tmp1[3],tmp2[3]={0,0,1.};
  double a;
  vectprojy(u,tmp1);
  a=vectangle(tmp1,tmp2);
  if(tmp1[1]<0) return(-1*a);
  return(a);
}
   

double fppangle_x(double *fpvec,double *fppvec){
  double tmp1[3],tmp2[3],a;
  vectprojx(fpvec,tmp1);
  vectprojx(fppvec,tmp2);
  a=vectangle(tmp1,tmp2);
  if((tmp1[2]*tmp2[0]-tmp1[0]*tmp2[2])<0) return(-1*a);
  return(a);
}

double fppangle_y(double *fpvec,double *fppvec){
  double tmp1[3],tmp2[3],a;
  vectprojy(fpvec,tmp1);
  vectprojy(fppvec,tmp2);
  a=vectangle(tmp1,tmp2);
  if((tmp1[1]*tmp2[2]-tmp1[2]*tmp2[1])>0) return(-1*a);
  return(a);
}










