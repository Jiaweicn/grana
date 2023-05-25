
#if 0  /* Original */
/** Unit and convention  *********************
   x:[mm]: distance from central ray in VDC-fixed frame.
   y:[mm]: distance from central ray in VDC-fixed frame.
   a:    : dx/dz in VDC-fixed frame.
   b:    : dy/dz in VDC-fixed frame.
*************************************************/



      subroutine opticsl2(x,a,y,b,x0,a0,y0,b0)
      implicit none
c
c X0 and A0
c    Ishida matrix based on Jul04 Exp.
c Y0 and B0
c    Matsuoka matrix (same as opticsl)
c Const.
      real*4    pi
      parameter(pi=3.1415926535e+00)
      real*4    deg2r
      parameter(deg2r=pi/180.0e+00)
      real*4    vdcangl
      parameter(vdcangl=54.76e+00)
      integer*4 nxmat,namat,nymat,nbmat
      parameter(nxmat=42,namat=35,nymat=42,nbmat=42)
c
      real*4    x,a,y,b
      real*4    x5,a5,a3,y5,b5
      real*4    x0,a0,y0,b0
c
      integer*4 ixmat(nxmat),iamat(namat),iymat(nymat),ibmat(nbmat)
      real*4     xmat(nxmat), amat(namat), ymat(nymat), bmat(nbmat)
      real*4    xx,aa,yy,bb
      integer*4 i
c
      data ixmat/
     &      0, 1000,  100,   10,
     &      1, 2000, 1100, 1010,
     &   1001,  200,  110,  101,
     &     20,   11,    2, 3000,
     &   2100, 2010, 2001, 1200,
     &   1110, 1101, 1020, 1011,
     &   1002,  300,  210,  201,
     &    120,  111,  102,   30,
     &     21,   12,    3, 4000,
     &   3100, 2200, 1300,  400,
     &     40,    4/
      data  iamat/
     &      0, 1000,  100,   10,
     &      1, 2000, 1100, 1010,
     &   1001,  200,  110,  101,
     &     20,   11,    2, 3000,
     &   2100, 2010, 2001, 1200,
     &   1110, 1101, 1020, 1011,
     &   1002,  300,  210,  201,
     &    120,  111,  102,   30,
     &     21,   12,    3/
      data  iymat/
     &   0,1000, 100,  10,
     &   1,2000,1100,1010,
     &1001, 200, 110, 101,
     &  20,  11,   2,3000,
     &2100,2010,2001,1200,
     &1110,1101,1020,1011,
     &1002, 300, 210, 201,
     & 120, 111, 102,  30,
     &  21,  12,   3,4000,
     & 400,  40,   4,3100,
     &2200,1300/
      data  ibmat/
     &   0,1000, 100,  10,
     &   1,2000,1100,1010,
     &1001, 200, 110, 101,
     &  20,  11,   2,3000,
     &2100,2010,2001,1200,
     &1110,1101,1020,1011,
     &1002, 300, 210, 201,
     & 120, 111, 102,  30,
     &  21,  12,   3,4000,
     & 400,  40,   4,3100,
     &2200,1300/
c
      data xmat/
     &  3.03262E-02,  1.90708E-04, -2.74836E-03,  4.60381E-05,
     & -1.05681E-01, -1.63835E-08,  8.73863E-05,  3.39703E-08,
     & -7.20438E-05, -2.94080E-02, -7.51874E-05,  1.54162E-01,
     &  1.84256E-08,  1.57391E-04,  2.45443E-02,  1.09146E-11,
     &  9.13081E-09,  1.04477E-11, -1.82230E-08, -6.38806E-05,
     & -2.85042E-08,  5.14218E-05,  7.73464E-12,  1.01663E-07,
     &  1.98607E-05,  2.16605E-02,  3.04577E-05, -5.83113E-02,
     & -9.82558E-09, -1.18498E-04, -2.43762E-02,  2.87345E-11,
     &  1.87894E-08, -6.30383E-05,  6.15223E-02,  3.27854E-14,
     &  6.86677E-13, -1.68539E-08,  1.92958E-05, -5.35479E-03,
     &  8.82364E-14,  3.14194E-02/
      data amat/
     &  4.03111E-01, -7.00011E-02,  4.47038E+02,  9.45354E-03,
     &  3.22769E+00,  1.20025E-06,  1.11250E-01,  3.93326E-06,
     &  1.83094E-04, -2.56758E+01, -1.22586E-01,  2.44124E+01,
     &  1.85974E-04,  1.96977E-01, -4.19527E+00,  1.35884E-08,
     & -1.72596E-04, -5.13436E-08,  9.82762E-05,  8.42766E-01,
     &  2.10258E-04, -2.43934E-01,  2.38505E-07, -3.36285E-04,
     & -1.02679E-01, -1.55426E+03, -3.85027E-01,  5.73202E+01,
     & -1.47484E-03,  2.83602E+00,  9.88270E+01, -1.88477E-07,
     & -2.87690E-03, -2.46669E-01, -5.78926E+01/
      data ymat/
     &-0.84223E-01, 0.45148E-01,-0.16377E-02,-0.14482E-01,
     &-0.11399E-01, 0.19320E-03,-0.33623E-04, 0.16439E-02,
     &-0.17256E-02, 0.48350E-05,-0.44283E-03, 0.94045E-05,
     & 0.12037E-01,-0.21535E-02,-0.61077E-03,-0.12105E-04,
     & 0.11516E-05, 0.68462E-04,-0.30662E-04, 0.47877E-07,
     & 0.20905E-04,-0.76543E-05,-0.37312E-03, 0.33096E-03,
     &-0.56867E-04, 0.45945E-08, 0.29832E-06, 0.49319E-06,
     & 0.59237E-04,-0.61601E-04, 0.10061E-04,-0.31620E-02,
     & 0.27348E-02,-0.54994E-03, 0.14465E-04,-0.18172E-07,
     &-0.50018E-09,-0.51986E-04, 0.30670E-06, 0.19579E-07,
     &-0.21132E-07, 0.64727E-08/
      data bmat/
     & 0.62562E+00, 0.29275E-01, 0.56947E-02, 0.17660E+02,
     &-0.62209E+01,-0.67896E-03, 0.94355E-03,-0.79147E-01,
     & 0.77036E-02,-0.21939E-03, 0.34606E-01,-0.68635E-02,
     &-0.22079E-02,-0.28546E-01, 0.21206E-02, 0.46327E-05,
     &-0.22098E-05, 0.38703E-03,-0.59769E-04, 0.20993E-05,
     &-0.21615E-03, 0.93849E-05,-0.42995E-02, 0.35616E-02,
     &-0.74629E-03,-0.76768E-06,-0.29180E-04, 0.11355E-04,
     &-0.57781E-03, 0.16075E-03, 0.55664E-05, 0.14887E+00,
     &-0.17445E+00, 0.67447E-01,-0.82223E-02, 0.73318E-07,
     & 0.49546E-08, 0.47293E-03,-0.23897E-05,-0.18904E-06,
     & 0.142           46E-06,-0.46246E-07/
c
c for X-A
c
      x5 = x
      a5 = a
      a3 = tan(atan(a)-vdcangl*deg2r)
      y5 = y
      b5 = b
      x0=0.
      a0=0.
      do i=1,nxmat ! (x5,a5,y5,b5) -> momentum deviation on target
         xx=x5**(ixmat(i)/1000)
         aa=a5**(mod(ixmat(i),1000)/100)
         yy=y5**(mod(ixmat(i),100)/10)
         bb=b5**(mod(ixmat(i),10))
         x0=x0+xmat(i)*xx*aa*yy*bb
      end do
      do i=1,namat ! (x5,"a3",y5,b5) -> a on target
         xx=x5**(iamat(i)/1000)
         aa=a3**(mod(iamat(i),1000)/100)
         yy=y5**(mod(iamat(i),100)/10)
         bb=b5**(mod(iamat(i),10))
         a0=a0+amat(i)*xx*aa*yy*bb
      end do
c
c for Y-B
c
      x5=x/10.                        ! mm  -> cm
      a5=(atan(a)-vdcangl*deg2r)*1000. ! rad -> mr
      y5=y/10.                        ! mm  -> cm
      b5=atan(b*cos(atan(a)))*1000.   ! rad -> mr
      y0=0.
      b0=0.
      do i=1,nymat ! (x5,a5,y5,b5) -> y on target
         xx=x5**(    iymat(i)/1000)
         aa=a5**(mod(iymat(i),1000)/100)
         yy=y5**(mod(iymat(i),100)/10)
         bb=b5**(mod(iymat(i),10))
         y0=y0+ymat(i)*xx*aa*yy*bb
      end do
      do i=1,nbmat ! (x5,a5,y5,b5) -> b on target
         xx=x5**(    ibmat(i)/1000)
         aa=a3**(mod(ibmat(i),1000)/100)
         yy=y5**(mod(ibmat(i),100)/10)
         bb=b5**(mod(ibmat(i),10))
         b0=b0+bmat(i)*xx*aa*yy*bb
      end do
      return
      end
#endif


/* lasmat.c 08/09/28 */
/*    Routine to perform raytrace of LAS events  */

#include <stdio.h>
#include <math.h>

#include <ana10.h>
#include "config.h"

#define N_ELEMENT_X 42
#define N_ELEMENT_A 35
#define N_ELEMENT_Y 42
#define N_ELEMENT_B 42

#define FAC_X 4000.0
#define FAC_A 1000.0

extern struct vdcdata lasfx;

void lasmat(double *fppos,double *colpos,double *theta,double *phi){
  double x0,xc=0,y0,yc=0,a0,ac=0,b0,bc=0,a3;

  struct mat {
    int i;
    double element;
  };

  static int ixmat[N_ELEMENT_X]=    {
       0, 1000,  100,   10,
       1, 2000, 1100, 1010,
    1001,  200,  110,  101,
      20,   11,    2, 3000,
    2100, 2010, 2001, 1200,
    1110, 1101, 1020, 1011,
    1002,  300,  210,  201,
     120,  111,  102,   30,
      21,   12,    3, 4000,
    3100, 2200, 1300,  400,
      40,    4};

  static int iamat[N_ELEMENT_A]={
       0, 1000,  100,   10,
       1, 2000, 1100, 1010,
    1001,  200,  110,  101,
      20,   11,    2, 3000,
    2100, 2010, 2001, 1200,
    1110, 1101, 1020, 1011,
    1002,  300,  210,  201,
     120,  111,  102,   30,
      21,   12,    3};

  static int iymat[N_ELEMENT_Y]={
       0, 1000,  100,   10,
       1, 2000, 1100, 1010,
    1001,  200,  110,  101,
      20,   11,    2, 3000,
    2100, 2010, 2001, 1200,
    1110, 1101, 1020, 1011,
    1002,  300,  210,  201,
     120,  111,  102,   30,
      21,   12,    3, 4000,
     400,   40,    4, 3100,
    2200, 1300};

  static int ibmat[N_ELEMENT_B]={
       0, 1000,  100,   10,
       1, 2000, 1100, 1010,
    1001,  200,  110,  101,
      20,   11,    2, 3000,
    2100, 2010, 2001, 1200,
    1110, 1101, 1020, 1011,
    1002,  300,  210,  201,
     120,  111,  102,   30,
      21,   12,    3, 4000,
     400,   40,    4, 3100,
    2200, 1300};

  static double xmat[N_ELEMENT_X]={
    3.03262E-02,  1.90708E-04, -2.74836E-03,  4.60381E-05,
   -1.05681E-01, -1.63835E-08,  8.73863E-05,  3.39703E-08,
   -7.20438E-05, -2.94080E-02, -7.51874E-05,  1.54162E-01,
    1.84256E-08,  1.57391E-04,  2.45443E-02,  1.09146E-11,
    9.13081E-09,  1.04477E-11, -1.82230E-08, -6.38806E-05,
   -2.85042E-08,  5.14218E-05,  7.73464E-12,  1.01663E-07,
    1.98607E-05,  2.16605E-02,  3.04577E-05, -5.83113E-02,
   -9.82558E-09, -1.18498E-04, -2.43762E-02,  2.87345E-11,
    1.87894E-08, -6.30383E-05,  6.15223E-02,  3.27854E-14,
    6.86677E-13, -1.68539E-08,  1.92958E-05, -5.35479E-03,
    8.82364E-14,  3.14194E-02};

  static double amat[N_ELEMENT_A]={
    4.03111E-01, -7.00011E-02,  4.47038E+02,  9.45354E-03,
    3.22769E+00,  1.20025E-06,  1.11250E-01,  3.93326E-06,
    1.83094E-04, -2.56758E+01, -1.22586E-01,  2.44124E+01,
    1.85974E-04,  1.96977E-01, -4.19527E+00,  1.35884E-08,
   -1.72596E-04, -5.13436E-08,  9.82762E-05,  8.42766E-01,
    2.10258E-04, -2.43934E-01,  2.38505E-07, -3.36285E-04,
   -1.02679E-01, -1.55426E+03, -3.85027E-01,  5.73202E+01,
   -1.47484E-03,  2.83602E+00,  9.88270E+01, -1.88477E-07,
   -2.87690E-03, -2.46669E-01, -5.78926E+01};

  static double ymat[N_ELEMENT_Y]={
   -0.84223E-01, 0.45148E-01,-0.16377E-02,-0.14482E-01,
   -0.11399E-01, 0.19320E-03,-0.33623E-04, 0.16439E-02,
   -0.17256E-02, 0.48350E-05,-0.44283E-03, 0.94045E-05,
    0.12037E-01,-0.21535E-02,-0.61077E-03,-0.12105E-04,
    0.11516E-05, 0.68462E-04,-0.30662E-04, 0.47877E-07,
    0.20905E-04,-0.76543E-05,-0.37312E-03, 0.33096E-03,
   -0.56867E-04, 0.45945E-08, 0.29832E-06, 0.49319E-06,
    0.59237E-04,-0.61601E-04, 0.10061E-04,-0.31620E-02,
    0.27348E-02,-0.54994E-03, 0.14465E-04,-0.18172E-07,
   -0.50018E-09,-0.51986E-04, 0.30670E-06, 0.19579E-07,
   -0.21132E-07, 0.64727E-08};

  static double bmat[N_ELEMENT_B]={
     0.62562E+00, 0.29275E-01, 0.56947E-02, 0.17660E+02,
    -0.62209E+01,-0.67896E-03, 0.94355E-03,-0.79147E-01,
     0.77036E-02,-0.21939E-03, 0.34606E-01,-0.68635E-02,
    -0.22079E-02,-0.28546E-01, 0.21206E-02, 0.46327E-05,
    -0.22098E-05, 0.38703E-03,-0.59769E-04, 0.20993E-05,
    -0.21615E-03, 0.93849E-05,-0.42995E-02, 0.35616E-02,
    -0.74629E-03,-0.76768E-06,-0.29180E-04, 0.11355E-04,
    -0.57781E-03, 0.16075E-03, 0.55664E-05, 0.14887E+00,
    -0.17445E+00, 0.67447E-01,-0.82223E-02, 0.73318E-07,
     0.49546E-08, 0.47293E-03,-0.23897E-05,-0.18904E-06,
     0.14246E-06,-0.46246E-07};


  int i;
  double pos[3];

  rotate_y(-lasfx.tilt_s,lasfx.tilt_c,fppos,pos);

  x0=pos[0];
  y0=pos[1]+50.0;
  a0=tan((*theta-(LAS_TILT))*D_TO_R);
  a3=tan(*theta*D_TO_R);
  b0=tan(*phi*D_TO_R);

/** Unit and convention  *********************
   For XA matrix

   x0:[mm]: distance from central ray in VDC-fixed frame.
   y0:[mm]: distance from central ray in VDC-fixed frame.
   a0:    : dx/dz in VDC-fixed frame.
   b0:    : dy/dz in VDC-fixed frame.
   a3:    : dx/dz from central ray
*************************************************/

#if 0
  fprintf(stdout,"x0:%f\n",x0);
  fprintf(stdout,"a0:%f\n",a0);
  fprintf(stdout,"y0:%f\n",y0);
  fprintf(stdout,"b0:%f\n",b0);
#endif

  for(i=0;i<N_ELEMENT_X;i++){
    xc+=(xmat[i]
	 *pow(x0,(double)(ixmat[i]/1000))
	 *pow(a0,(double)((ixmat[i]%1000)/100))
	 *pow(y0,(double)((ixmat[i]%100)/10))  
	 *pow(b0,(double)(ixmat[i]%10)));      
  }

  for(i=0;i<N_ELEMENT_A;i++){
    ac+=(amat[i]
	 *pow(x0,(double)(iamat[i]/1000))
	 *pow(a3,(double)((iamat[i]%1000)/100))
 	 *pow(y0,(double)((iamat[i]%100)/10))
	 *pow(b0,(double)(iamat[i]%10)));
#if 0
    fprintf(stdout,"%4d  %14.5e\n",iamat[i],amat[i]);
    fprintf(stdout,"%2d:x:%f\n",i,(double)(iamat[i]/1000));
    fprintf(stdout,"%2d:a:%f\n",i,(double)((iamat[i]%1000)/100));
    fprintf(stdout,"%2d:y:%f\n",i,(double)((iamat[i]%100)/10));
    fprintf(stdout,"%2d:b:%f\n",i,(double)(iamat[i]%10));
    fprintf(stdout,"%7.3f   %7.3f\n",(amat[i]
    *pow(x0,(double)(iamat[i]/1000))
    *pow(a0,(double)((iamat[i]%1000)/100))
    *pow(y0,(double)((iamat[i]%100)/10))  
    *pow(b0,(double)(iamat[i]%10))),ac);      
    fprintf(stdout,"\n\n");
#endif
  }

/** Unit and convention  *********************
   For YB matrix

   x0:[cm]: distance from central ray in VDC-fixed frame.
   y0:[cm]: distance from central ray in VDC-fixed frame.
   a0:    : mr in VDC-fixed frame.
   b0:    : mr dy/dz in VDC-fixed frame.
   a3:    : dx/dz from central ray
*************************************************/

  x0=pos[0]/10.0;
  y0=pos[1]/10.0+3.0;
  a3=(*theta*D_TO_R)*1000.0;
  a0=(*theta-(LAS_TILT))*D_TO_R*1000.0;
  b0=(*phi*D_TO_R)*1000.0;

  for(i=0;i<N_ELEMENT_Y;i++){
    yc+=(ymat[i]
	 *pow(x0,(double)(iymat[i]/1000))
	 *pow(a3,(double)((iymat[i]%1000)/100))
	 *pow(y0,(double)((iymat[i]%100)/10))  
	 *pow(b0,(double)(iymat[i]%10)));      
  }

  for(i=0;i<N_ELEMENT_B;i++){
    bc+=(bmat[i]
	 *pow(x0,(double)(ibmat[i]/1000))
	 *pow(a3,(double)((ibmat[i]%1000)/100))
	 *pow(y0,(double)((ibmat[i]%100)/10))
	 *pow(b0,(double)(ibmat[i]%10)));
  }

  xc*=100.0;
  ac*=(R_TO_D/1000.0);
  bc*=(R_TO_D/1000.0);

  *colpos=xc;
  *(colpos+1)=yc;
  *(theta+1)=ac;
  *(phi+1)=bc;
}


