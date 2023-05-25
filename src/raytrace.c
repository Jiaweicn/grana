/** raytrace.c  ***/
/* 00/06/22  rotate_y で y座標がゼロになる Bug を Fix */
/* 00/05/12  add nclude file from config.h  */

#include <stdio.h>
#include <math.h>

#include "ana10.h"
#include "config.h"
#include "vectcalc.h"


#define N 3

void makeplane(double **plane){
  int i;
  for(i=0;i<NPL;i++){
    *(*(plane+i)+3)=(*(*(plane+i)+3))*(*(*(plane+i)+2));
  }
}
  
void plane_eq(double *rayvec,double *wiredire,double *point,double *eq){
  double normal[3];
  vectvecpro(wiredire,rayvec,normal);
  *eq=*normal;
  *(eq+1)=*(normal+1);
  *(eq+2)=*(normal+2);
  *(eq+3)=vectscpro(normal,point);
}

void rotate_y(double sin,double cos,double *point,double *rpoint){
  double tmp[3];
  tmp[0]=*point*cos+*(point+2)*sin;
  tmp[1]=*(point+1);
  tmp[2]=-(*point)*sin+*(point+2)*cos;
  *rpoint=tmp[0];
  *(rpoint+1)=tmp[1];
  *(rpoint+2)=tmp[2];
}

void fpp_rotate_x(double theta_x,double *point,double *rpoint){
  double tmp[3];
  tmp[1]=*(point+1)*cos(theta_x*D_TO_R)+*(point+2)*sin(theta_x*D_TO_R);
  tmp[2]=-(*(point+1))*sin(theta_x*D_TO_R)+*(point+2)*cos(theta_x*D_TO_R);
  *(rpoint+1)=tmp[1];
  *(rpoint+2)=tmp[2];
}

void fpp_rotate_y(double theta_y,double *point,double *rpoint){
  double tmp[3];
  tmp[0]=*(point)*cos(theta_y*D_TO_R)-*(point+2)*sin(theta_y*D_TO_R);
  tmp[2]=*(point)*sin(theta_y*D_TO_R)+*(point+2)*cos(theta_y*D_TO_R);
  *(rpoint)=tmp[0];
  *(rpoint+2)=tmp[2];
}


int calcpoint(double *route_a,double *route_b,double *plain,double *point){
  static double a[N][N+1];
  double p,d,max,dumy;
  int i,j,k,s;

  for(i=0;i<N+1;i++){
    a[0][i]=*(route_a+i);
    a[1][i]=*(route_b+i);
    a[2][i]=*(plain+i);
  }
  for (k=0;k<N;k++){
    max=0;s=k;
    for(j=k;j<N;j++){
      if (fabs(a[j][k])>max) { /* compare coefficients by absolute value */
	max=fabs(a[j][k]);s=j;
      }
    }
    if (max==0){
      printf("We cannot solve this equation!\n");
      return(1);
    }
    for (j=0;j<=N;j++){
      dumy=a[k][j];
      a[k][j]=a[s][j];
      a[s][j]=dumy;
    }

    p=a[k][k]; /* pivot coefficient */
    for(j=k;j<N+1;j++)
      a[k][j]=a[k][j]/p; /* pivot line was divided by pivot coefficient */
    for(i=0;i<N;i++){ 
      if (i!=k) {
	d=a[i][k];
	for(j=k;j<N+1;j++)
	  a[i][j]=a[i][j]-d*a[k][j];
      }
    }
  }

  for(k=0;k<N;k++)
    *(point+k)=a[k][N];
  return(0);
}


int calcvertex(double a[2][3],double *answer){
  double p,d,max,dumy;
  int i,j,k,s;

  for (k=0;k<2;k++){
    max=0;s=k;
    for(j=k;j<2;j++){
      if (fabs(a[j][k])>max) { /* compare coefficients by absolute value */
	max=fabs(a[j][k]);s=j;
      }
    }
    if (max==0){
      printf("We cannot calculate vertex point!\n");
      return(1);
    }
    for (j=0;j<=2;j++){
      dumy=a[k][j];
      a[k][j]=a[s][j];
      a[s][j]=dumy;
    }

    p=a[k][k]; /* pivot coefficient */
    for(j=k;j<3;j++)
      a[k][j]=a[k][j]/p; /* pivot line was divided by pivot coefficient */
    for(i=0;i<2;i++){ 
      if (i!=k) {
	d=a[i][k];
	for(j=k;j<3;j++)
	  a[i][j]=a[i][j]-d*a[k][j];
      }
    }
  }

  for(k=0;k<2;k++)
    *(answer+k)=a[k][2];
  return(0);
}

