/* lasmat.c 00/01/25 */
/*    Routine to perform raytrace of LAS events  */

#include <stdio.h>
#include <math.h>

#include "ana9.h"
#include "config.h"

#define N_ELEMENT 42

#define FAC_X 4000.0
#define FAC_A 1000.0

extern struct vdcdata lasfx;

void lasmat(double *fppos,double *colpos,double *theta,double *phi){
  double x0,xc=0,y0,yc=0,a0,ac=0,b0,bc=0;

  struct mat {
    int i;
    double element;
  };

  static struct mat xmat[N_ELEMENT]={
    {   0,  -0.18661E+00},{1000,   0.23809E+00},{ 100,  -0.47531E-02},
    {  10,  -0.60432E-03},{   1,   0.43885E-04},{2000,  -0.18881E-03},
    {1100,   0.68227E-04},{1010,   0.27744E-04},{1001,  -0.80668E-05},
    { 200,  -0.11405E-04},{ 110,  -0.78273E-05},{ 101,   0.62122E-06},
    {  20,   0.15047E-02},{  11,   0.10310E-03},{   2,  -0.10466E-03},
    {3000,   0.16259E-05},{2100,  -0.11915E-05},{2010,  -0.27064E-06},
    {2001,   0.92677E-07},{1200,   0.21249E-06},{1110,   0.20590E-06},
    {1101,  -0.93046E-07},{1020,  -0.13356E-04},{1011,  -0.12164E-05},
    {1002,   0.94939E-06},{ 300,  -0.26518E-07},{ 210,   0.12238E-07},
    { 201,   0.32775E-08},{ 120,  -0.24516E-04},{ 111,   0.16609E-04},
    { 102,  -0.25803E-05},{  30,   0.20905E-04},{  21,  -0.22338E-04},
    {  12,   0.79290E-05},{   3,  -0.92803E-06},{4000,  -0.31164E-08},
    { 400,  -0.40807E-10},{  40,   0.19344E-05},{   4,  -0.66008E-08},
    {3100,   0.18250E-08},{2200,  -0.13502E-08},{1300,   0.39401E-09}
  };

  static struct mat amat[N_ELEMENT]={
    {   0,  -0.42862E+00},{1000,   0.64956E+00},{ 100,  -0.42364E+00},
    {  10,   0.14198E-01},{   1,  -0.48221E-03},{2000,   0.42551E-04},
    {1100,  -0.10024E-02},{1010,  -0.71488E-03},{1001,   0.27941E-03},
    { 200,  -0.10286E-04},{ 110,   0.13660E-03},{ 101,  -0.24261E-04},
    {  20,  -0.11750E+00},{  11,   0.84813E-01},{   2,  -0.15667E-01},
    {3000,  -0.59102E-05},{2100,   0.71903E-05},{2010,   0.65433E-06},
    {2001,   0.56182E-06},{1200,  -0.31274E-05},{1110,  -0.17598E-05},
    {1101,   0.59424E-06},{1020,  -0.86304E-04},{1011,   0.31027E-03},
    {1002,  -0.10052E-03},{ 300,   0.40568E-06},{ 210,  -0.44650E-06},
    { 201,   0.10146E-06},{ 120,   0.25099E-03},{ 111,  -0.21618E-03},
    { 102,   0.45582E-04},{  30,  -0.18232E-03},{  21,   0.26900E-03},
    {  12,  -0.14139E-03},{   3,   0.21316E-04},{4000,   0.13234E-07},
    { 400,   0.38781E-09},{  40,  -0.12192E-04},{   4,   0.34280E-07},
    {3100,  -0.10815E-07},{2200,   0.41372E-08},{1300,  -0.20685E-08}
  };

  static struct mat ymat[N_ELEMENT]={
    {   0,   0.37489E-03},{1000,  -0.39236E-04},{ 100,   0.13796E-04},
    {  10,  -0.23170E-01},{   1,  -0.27979E-01},{2000,   0.68248E-06},
    {1100,  -0.10181E-05},{1010,   0.34619E-02},{1001,  -0.15538E-02},
    { 200,   0.73775E-07},{ 110,  -0.36720E-04},{ 101,   0.10176E-03},
    {  20,   0.37190E-04},{  11,   0.13998E-03},{   2,   0.59186E-05},
    {3000,   0.46347E-08},{2100,  -0.12921E-07},{2010,   0.79679E-05},
    {2001,  -0.64106E-05},{1200,   0.95755E-08},{1110,   0.47271E-05},
    {1101,  -0.12522E-06},{1020,   0.12741E-05},{1011,  -0.89648E-05},
    {1002,   0.27324E-05},{ 300,  -0.16178E-08},{ 210,  -0.27235E-05},
    { 201,   0.66060E-06},{ 120,   0.85253E-06},{ 111,  -0.20346E-06},
    { 102,  -0.16351E-06},{  30,  -0.27646E-02},{  21,   0.31975E-02},
    {  12,  -0.12013E-02},{   3,   0.14325E-03},{4000,  -0.14843E-10},
    { 400,   0.00000E+00},{  40,   0.92355E-06},{   4,  -0.50000E-08},
    {3100,   0.23278E-09},{2200,  -0.87003E-10},{1300,   0.00000E+00}
  };

  static struct mat bmat[N_ELEMENT]={
    {   0,  -0.78400E-02},{1000,   0.96756E-03},{ 100,  -0.39065E-03},
    {  10,   0.20181E+02},{   1,  -0.72797E+01},{2000,  -0.12850E-04},
    {1100,   0.13378E-04},{1010,  -0.60818E-01},{1001,   0.60173E-02},
    { 200,  -0.13272E-05},{ 110,   0.30303E-01},{ 101,  -0.74348E-02},
    {  20,   0.19660E-02},{  11,  -0.21080E-02},{   2,   0.39807E-04},
    {3000,  -0.35574E-06},{2100,   0.48120E-06},{2010,  -0.47979E-04},
    {2001,   0.13431E-03},{1200,  -0.22985E-06},{1110,  -0.47552E-04},
    {1101,  -0.57569E-04},{1020,  -0.11313E-03},{1011,   0.14457E-03},
    {1002,  -0.32234E-04},{ 300,   0.34873E-07},{ 210,  -0.10437E-04},
    { 201,   0.85366E-05},{ 120,  -0.20252E-05},{ 111,  -0.34856E-05},
    { 102,   0.21970E-05},{  30,   0.24521E-01},{  21,  -0.28313E-01},
    {  12,   0.10421E-01},{   3,  -0.12063E-02},{4000,   0.32720E-08},
    { 400,   0.12487E-09},{  40,  -0.64781E-05},{   4,   0.64861E-07},
    {3100,  -0.43619E-08},{2200,   0.19122E-08},{1300,   0.00000E+00}
  };


/** Unit and convention  *********************
   x0:[cm]: distance from central ray in VDC-fixed frame.
   y0:[cm]: distance from central ray in VDC-fixed frame.
   a0:[mrad]: angle from central ray
   b0:[mard]: angle from central ray
*************************************************/

  int i;
  double pos[3];
  rotate_y(-lasfx.tilt_s,lasfx.tilt_c,fppos,pos);

  /* noro  */
  /*
  x0=pos[0];
  y0=pos[1];
  a0=tan((*theta-(LAS_TILT))*D_TO_R);
  b0=tan(*phi*D_TO_R);*/


/*  fprintf(stdout,"x0:%f\n",x0);
  fprintf(stdout,"a0:%f\n",a0);
  fprintf(stdout,"y0:%f\n",y0);
  fprintf(stdout,"b0:%f\n",b0);*/

  /* matsuoka */
  /*
  x0=fppos[0]/10.0;
  y0=fppos[1]/10.0;
  a0=(*theta)*D_TO_R*1000.0;
  b0=(*phi)*D_TO_R*1000.0;*/

  x0=pos[0]/10.0;
  y0=pos[1]/10.0;
  a0=(*theta)*D_TO_R*1000.0;
  b0=(*phi)*D_TO_R*1000.0;

  for(i=0;i<N_ELEMENT;i++){
    xc+=(xmat[i].element
	 *pow(x0,(double)(xmat[i].i/1000))
	 *pow(a0,(double)((xmat[i].i%1000)/100))
	 *pow(y0,(double)((xmat[i].i%100)/10))  
	 *pow(b0,(double)(xmat[i].i%10)));      
    ac+=(amat[i].element
	 *pow(x0,(double)(amat[i].i/1000))
	 *pow(a0,(double)((amat[i].i%1000)/100))
	 *pow(y0,(double)((amat[i].i%100)/10))
	 *pow(b0,(double)(amat[i].i%10)));
    yc+=(xmat[i].element
	 *pow(x0,(double)(ymat[i].i/1000))
	 *pow(a0,(double)((ymat[i].i%1000)/100))
	 *pow(y0,(double)((ymat[i].i%100)/10))  
	 *pow(b0,(double)(ymat[i].i%10)));      
    bc+=(amat[i].element
	 *pow(x0,(double)(bmat[i].i/1000))
	 *pow(a0,(double)((bmat[i].i%1000)/100))
	 *pow(y0,(double)((bmat[i].i%100)/10))
	 *pow(b0,(double)(bmat[i].i%10)));

/*  fprintf(stdout,"%2d:x:%f\n",i,(double)(xmat[i].i/1000));
  fprintf(stdout,"%2d:a:%f\n",i,(double)((xmat[i].i%1000)/100));
  fprintf(stdout,"%2d:y:%f\n",i,(double)((xmat[i].i%100)/10));
  fprintf(stdout,"%2d:b:%f\n",i,(double)(xmat[i].i%10));
  fprintf(stdout,"\n\n",(double)(xmat[i].i%10));*/
  }

  /* noro */
  /*
  xc*=3000;
  yc=*(fppos+1);
  ac*=(R_TO_D/1000.);
  bc=*phi; */

  /* matsuoka */
#if 0
  xc*=10.0;
  yc*=10.0;
#endif
  ac*=(R_TO_D/1000.);
  bc*=(R_TO_D/1000.);

/*  fprintf(stdout,"xc:%f\n",xc);
  fprintf(stdout,"ac:%f\n",ac);
  fprintf(stdout,"yc:%f\n",yc);
  fprintf(stdout,"bc:%f\n",bc);
  fprintf(stdout,"\n\n");*/

  *colpos=xc;
  *(colpos+1)=yc;
  *(theta+1)=ac;
  *(phi+1)=bc;
}

/*********************************************************************:
 *******************************************************************   
c	focal plane 

        	x0(mm) x-position
	        y0(mm) y-position
	        a0=dx/dz
	        b0=dy/dz

	deg=pi/180
	vdcang=54.04

	x00=x0/10.                            ! X00(cm)
        aa=atan(a0)                           ! a0=tan(theta)
	a00=(aa-vdcang*deg)*1000.             ! A00(mrad)
	y00=y0/10.                            ! Y00(cm)
	bb0=b0*cos(aa)                        ! B00(mrad),b0=tan(phi)
	b00=atan(bb0)*1000.

	do i=1,50
	  if(itmat(i).lt.0) goto 722
	  t = t + tmat(i) *x00**
	1 int(itmat(i)/1000)*a00**int((mod(itmat(i),
	1 1000))/100)*y00**int((mod(itmat(i),100))/10)
	1 *b00**mod(itmat(i),10)
	  enddo

722	a=0.
	aH=0.
	do i=1,50
	  if(iamat(i).t.0) goto 724
	  a = a + amat(i) *x00**int(iamat(i)/1000)
	1 *a00**int((mod(iamat(i),
	1 1000))/100)*y00**int((mod(iamat(i),100))/10)
	1  *b00**mod(iamat(i),10)
	  aH = aH + amat(i) *x00H**int(iamat(i)/1000)
	1 *a00H**int((mod(iamat(i),
	1 1000))/100)*y00H**int((mod(iamat(i),100))/10)
	1 *b00H**mod(iamat(i),10)
	  enddo

724	y=0.
	yH=0.
	do i=1,50
	  if(iymat(i).t.0) goto 726
	  y = y + ymat(i) *x00**int(iymat(i)/1000)
	1 *a00**int((mod(iymat(i),
	1 1000))/100)*y00**int((mod(iymat(i),100))/10)
	1 *b00**mod(iymat(i),10)
	  yH = yH + ymat(i) *x00H**int(iymat(i)/1000)
	1 *a00H**int((mod(iymat(i),
	1 1000))/100)*y00H**int((mod(iymat(i),100))/10)
	1  *b00H**mod(iymat(i),10)
	  enddo

726	b=0. 
	bH=0. 
	do i=1,50
	  if(ibmat(i).lt.0) goto 728
	  b = b + bmat(i) *x00**int(ibmat(i)/1000)
	1 *a00**int((mod(ibmat(i),
	1 1000))/100)*y00**int((mod(ibmat(i),100))/10)
	1 *b00**mod(ibmat(i),10)
	  bH = bH + bmat(i) *x00H**int(ibmat(i)/1000)
	1 *a00H**int((mod(ibmat(i),
	1 1000))/100)*y00H**int((mod(ibmat(i),100))/10)
	1  *b00H**mod(ibmat(i),10)
	  enddo


c   On target 


728	iev(211)=intr(100*t)     		! [1]211 : t (0.01%)
	iev(212)=intr(a)			! [1]219 : a (mrad) 横
	iev(213)=intr(100*y)			! [1]213 : y (0.1mm)
	iev(214)=intr(b)			! [1]214 : b (mrad) 縦


       です。
       これでわかるでしょうーか。
       前田

matrixは、


itmat( 1)=         0
tmat( 1)=-0.18661E+00
itmat( 2)=      1000
tmat( 2)= 0.23809E+00
itmat( 3)=       100
tmat( 3)=-0.47531E-02
itmat( 4)=        10
tmat( 4)=-0.60432E-03
itmat( 5)=         1
tmat( 5)= 0.43885E-04
itmat( 6)=      2000
tmat( 6)=-0.18881E-03
itmat( 7)=      1100
tmat( 7)= 0.68227E-04
itmat( 8)=      1010
tmat( 8)= 0.27744E-04
itmat( 9)=      1001
tmat( 9)=-0.80668E-05
itmat(10)=       200
tmat(10)=-0.11405E-04
itmat(11)=       110
tmat(11)=-0.78273E-05
itmat(12)=       101
tmat(12)= 0.62122E-06
itmat(13)=        20
tmat(13)= 0.15047E-02
itmat(14)=        11
tmat(14)= 0.10310E-03
itmat(15)=         2
tmat(15)=-0.10466E-03
itmat(16)=      3000
tmat(16)= 0.16259E-05
itmat(17)=      2100
tmat(17)=-0.11915E-05
itmat(18)=      2010
tmat(18)=-0.27064E-06
itmat(19)=      2001
tmat(19)= 0.92677E-07
itmat(20)=      1200
tmat(20)= 0.21249E-06
itmat(21)=      1110
tmat(21)= 0.20590E-06
itmat(22)=      1101
tmat(22)=-0.93046E-07
itmat(23)=      1020
tmat(23)=-0.13356E-04
itmat(24)=      1011
tmat(24)=-0.12164E-05
itmat(25)=      1002
tmat(25)= 0.94939E-06
itmat(26)=       300
tmat(26)=-0.26518E-07
itmat(27)=       210
tmat(27)= 0.12238E-07
itmat(28)=       201
tmat(28)= 0.32775E-08
itmat(29)=       120
tmat(29)=-0.24516E-04
itmat(30)=       111
tmat(30)= 0.16609E-04
itmat(31)=       102
tmat(31)=-0.25803E-05
itmat(32)=        30
tmat(32)= 0.20905E-04
itmat(33)=        21
tmat(33)=-0.22338E-04
itmat(34)=        12
tmat(34)= 0.79290E-05
itmat(35)=         3
tmat(35)=-0.92803E-06
itmat(36)=      4000
tmat(36)=-0.31164E-08
itmat(37)=       400
tmat(37)=-0.40807E-10
itmat(38)=        40
tmat(38)= 0.19344E-05
itmat(39)=         4
tmat(39)=-0.66008E-08
itmat(40)=      3100
tmat(40)= 0.18250E-08
itmat(41)=      2200
tmat(41)=-0.13502E-08
itmat(42)=      1300
tmat(42)= 0.39401E-09
itmat(43)=        -1
iamat( 1)=         0
amat( 1)=-0.42862E+00
iamat( 2)=      1000
amat( 2)= 0.64956E+00
iamat( 3)=       100
amat( 3)=-0.42364E+00
iamat( 4)=        10
amat( 4)= 0.14198E-01
iamat( 5)=         1
amat( 5)=-0.48221E-03
iamat( 6)=      2000
amat( 6)= 0.42551E-04
iamat( 7)=      1100
amat( 7)=-0.10024E-02
iamat( 8)=      1010
amat( 8)=-0.71488E-03
iamat( 9)=      1001
amat( 9)= 0.27941E-03
iamat(10)=       200
amat(10)=-0.10286E-04
iamat(11)=       110
amat(11)= 0.13660E-03
iamat(12)=       101
amat(12)=-0.24261E-04
iamat(13)=        20
amat(13)=-0.11750E+00
iamat(14)=        11
amat(14)= 0.84813E-01
iamat(15)=         2
amat(15)=-0.15667E-01
iamat(16)=      3000
amat(16)=-0.59102E-05
iamat(17)=      2100
amat(17)= 0.71903E-05
iamat(18)=      2010
amat(18)= 0.65433E-06
iamat(19)=      2001
amat(19)= 0.56182E-06
iamat(20)=      1200
amat(20)=-0.31274E-05
iamat(21)=      1110
amat(21)=-0.17598E-05
iamat(22)=      1101
amat(22)= 0.59424E-06
iamat(23)=      1020
amat(23)=-0.86304E-04
iamat(24)=      1011
amat(24)= 0.31027E-03
iamat(25)=      1002
amat(25)=-0.10052E-03
iamat(26)=       300
amat(26)= 0.40568E-06
iamat(27)=       210
amat(27)=-0.44650E-06
iamat(28)=       201
amat(28)= 0.10146E-06
iamat(29)=       120
amat(29)= 0.25099E-03
iamat(30)=       111
amat(30)=-0.21618E-03
iamat(31)=       102
amat(31)= 0.45582E-04
iamat(32)=        30
amat(32)=-0.18232E-03
iamat(33)=        21
amat(33)= 0.26900E-03
iamat(34)=        12
amat(34)=-0.14139E-03
iamat(35)=         3
amat(35)= 0.21316E-04
iamat(36)=      4000
amat(36)= 0.13234E-07
iamat(37)=       400
amat(37)= 0.38781E-09
iamat(38)=        40
amat(38)=-0.12192E-04
iamat(39)=         4
amat(39)= 0.34280E-07
iamat(40)=      3100
amat(40)=-0.10815E-07
iamat(41)=      2200
amat(41)= 0.41372E-08
iamat(42)=      1300
amat(42)=-0.20685E-08
iamat(43)=        -1
iymat( 1)=         0
ymat( 1)= 0.37489E-03
iymat( 2)=      1000
ymat( 2)=-0.39236E-04
iymat( 3)=       100
ymat( 3)= 0.13796E-04
iymat( 4)=        10
ymat( 4)=-0.23170E-01
iymat( 5)=         1
ymat( 5)=-0.27979E-01
iymat( 6)=      2000
ymat( 6)= 0.68248E-06
iymat( 7)=      1100
ymat( 7)=-0.10181E-05
iymat( 8)=      1010
ymat( 8)= 0.34619E-02
iymat( 9)=      1001
ymat( 9)=-0.15538E-02
iymat(10)=       200
ymat(10)= 0.73775E-07
iymat(11)=       110
ymat(11)=-0.36720E-04
iymat(12)=       101
ymat(12)= 0.10176E-03
iymat(13)=        20
ymat(13)= 0.37190E-04
iymat(14)=        11
ymat(14)= 0.13998E-03
iymat(15)=         2
ymat(15)= 0.59186E-05
iymat(16)=      3000
ymat(16)= 0.46347E-08
iymat(17)=      2100
ymat(17)=-0.12921E-07
iymat(18)=      2010
ymat(18)= 0.79679E-05
iymat(19)=      2001
ymat(19)=-0.64106E-05
iymat(20)=      1200
ymat(20)= 0.95755E-08
iymat(21)=      1110
ymat(21)= 0.47271E-05
iymat(22)=      1101
ymat(22)=-0.12522E-06
iymat(23)=      1020
ymat(23)= 0.12741E-05
iymat(24)=      1011
ymat(24)=-0.89648E-05
iymat(25)=      1002
ymat(25)= 0.27324E-05
iymat(26)=       300
ymat(26)=-0.16178E-08
iymat(27)=       210
ymat(27)=-0.27235E-05
iymat(28)=       201
ymat(28)= 0.66060E-06
iymat(29)=       120
ymat(29)= 0.85253E-06
iymat(30)=       111
ymat(30)=-0.20346E-06
iymat(31)=       102
ymat(31)=-0.16351E-06
iymat(32)=        30
ymat(32)=-0.27646E-02
iymat(33)=        21
ymat(33)= 0.31975E-02
iymat(34)=        12
ymat(34)=-0.12013E-02
iymat(35)=         3
ymat(35)= 0.14325E-03
iymat(36)=      4000
ymat(36)=-0.14843E-10
iymat(37)=       400
ymat(37)= 0.00000E+00
;ymat(37)=-0.43020E-11
iymat(38)=        40
ymat(38)= 0.92355E-06
iymat(39)=         4
ymat(39)=-0.50000E-08
iymat(40)=      3100
ymat(40)= 0.23278E-09
iymat(41)=      2200
ymat(41)=-0.87003E-10
iymat(42)=      1300
;ymat(42)= 0.30741E-10
ymat(42)= 0.00000E+00
iymat(43)=        -1
ibmat( 1)=         0
bmat( 1)=-0.78400E-02
ibmat( 2)=      1000
bmat( 2)= 0.96756E-03
ibmat( 3)=       100
bmat( 3)=-0.39065E-03
ibmat( 4)=        10
bmat( 4)= 0.20181E+02
ibmat( 5)=         1
bmat( 5)=-0.72797E+01
ibmat( 6)=      2000
bmat( 6)=-0.12850E-04
ibmat( 7)=      1100
bmat( 7)= 0.13378E-04
ibmat( 8)=      1010
bmat( 8)=-0.60818E-01
ibmat( 9)=      1001
bmat( 9)= 0.60173E-02
ibmat(10)=       200
bmat(10)=-0.13272E-05
ibmat(11)=       110
bmat(11)= 0.30303E-01
ibmat(12)=       101
bmat(12)=-0.74348E-02
ibmat(13)=        20
bmat(13)= 0.19660E-02
ibmat(14)=        11
bmat(14)=-0.21080E-02
ibmat(15)=         2
bmat(15)= 0.39807E-04
ibmat(16)=      3000
bmat(16)=-0.35574E-06
ibmat(17)=      2100
bmat(17)= 0.48120E-06
ibmat(18)=      2010
bmat(18)=-0.47979E-04
ibmat(19)=      2001
bmat(19)= 0.13431E-03
ibmat(20)=      1200
bmat(20)=-0.22985E-06
ibmat(21)=      1110
bmat(21)=-0.47552E-04
ibmat(22)=      1101
bmat(22)=-0.57569E-04
ibmat(23)=      1020
bmat(23)=-0.11313E-03
ibmat(24)=      1011
bmat(24)= 0.14457E-03
ibmat(25)=      1002
bmat(25)=-0.32234E-04
ibmat(26)=       300
bmat(26)= 0.34873E-07
ibmat(27)=       210
bmat(27)=-0.10437E-04
ibmat(28)=       201
bmat(28)= 0.85366E-05
ibmat(29)=       120
bmat(29)=-0.20252E-05
ibmat(30)=       111
bmat(30)=-0.34856E-05
ibmat(31)=       102
bmat(31)= 0.21970E-05
ibmat(32)=        30
bmat(32)= 0.24521E-01
ibmat(33)=        21
bmat(33)=-0.28313E-01
ibmat(34)=        12
bmat(34)= 0.10421E-01
ibmat(35)=         3
bmat(35)=-0.12063E-02
ibmat(36)=      4000
bmat(36)= 0.32720E-08
ibmat(37)=       400
bmat(37)= 0.12487E-09
ibmat(38)=        40
bmat(38)=-0.64781E-05
ibmat(39)=         4
bmat(39)= 0.64861E-07
ibmat(40)=      3100
bmat(40)=-0.43619E-08
ibmat(41)=      2200
bmat(41)= 0.19122E-08
ibmat(42)=      1300
;bmat(42)=-0.76813E-09
bmat(42)= 0.000000E+00
ibmat(43)=        -1
;
****************************************************************************/


