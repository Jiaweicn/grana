/* lasmat.c 00/01/25 */
/*    Routine to perform raytrace of LAS events  */

#include <stdio.h>
#include <math.h>

#include "ana9.h"
#include "config.h"

#define N_ELEMENT 41

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
    {   0,  0.228060E-01},{1000,  0.233920E-03},{ 100,  -.292140E-01},
    {  10,  0.904490E-05},{   1,  0.615280E-02},{2000,  0.650140E-07},
    {1100,  -.330890E-05},{1010,  -.107770E-07},{1001,  0.111670E-04},
    { 200,  0.163620E-01},{ 110,  -.764690E-05},{ 101,  -.149840E-01},
    {  20,  0.896520E-07},{  11,  -.224500E-04},{   2,  -.108740E-02},
    {3000,  0.496760E-11},{2100,  -.481540E-07},{2010,  -.449220E-11},
    {2001,  -.218910E-08},{1200,  -.776870E-05},{1110,  0.613150E-08},
    {1101,  -.833850E-05},{1020,  0.407430E-10},{1011,  0.209440E-07},
    {1002,  0.173090E-05},{ 300,  -.489810E-02},{ 210,  0.208410E-05},
    { 201,  0.532190E-02},{ 120,  0.326480E-08},{ 111,  0.117170E-04},
    { 102,  -.731200E-03},{  21,  0.125660E-06},{  12,  -.528590E-04},
    {   3,  0.351650E-01},{4000,  -.357350E-13},{ 400,  -.137010E-03},
    {  40,  -.660810E-11},{   4,  0.102860E-01},{3100,  0.300370E-10},
    {2200,  -.325960E-08},{1300,  0.750840E-05}};

  static struct mat amat[N_ELEMENT]={
    {   0,  -.908820E+03},{1000,  -.984020E+00},{ 100,  0.210780E+04},
    {  10,  -.813890E-02},{   1,  -.151760E+03},{2000,  -.475350E-03},
    {1100,  0.205390E+01},{1010,  0.820190E-05},{1001,  -.107190E+00},
    { 200,  -.208670E+04},{ 110,  -.390760E-02},{ 101,  0.230630E+03},
    {  20,  0.174410E-02},{  11,  0.171440E+00},{   2,  0.468810E+02},
    {3000,  -.128980E-06},{2100,  0.729530E-03},{2010,  -.506160E-09},
    {2001,  0.726870E-05},{1200,  -.153170E+01},{1110,  -.724340E-05},
    {1101,  0.764530E-01},{1020,  0.381520E-06},{1011,  0.449780E-04},
    {1002,  0.410380E-01},{ 300,  0.100450E+04},{ 210,  0.330790E-02},
    { 201,  -.767940E+02},{ 120,  -.942810E-03},{ 111,  -.389920E-01},
    { 102,  -.101890E+01},{  21,  -.629010E-03},{  12,  -.921750E-01},
    {   3,  -.215620E+03},{4000,  -.131130E-10},{ 400,  -.184170E+03},
    {  40,  -.230160E-07},{   4,  -.300980E+03},{3100,  0.976790E-07},
    {2200,  -.276040E-03},{1300,  0.380340E+00}};

/** Unit and convention  *********************
   x0:[mm]: distance from central ray in VDC-fixed frame.
   y0:[mm]: distance from central ray in VDC-fixed frame.
   a0:    : tan(dx/dz) in VDC-fixed frame.
   b0:    : tan(dy/dz) in VDC-fixed frame.
*************************************************/

  int i;
  double pos[3];
  rotate_y(-lasfx.tilt_s,lasfx.tilt_c,fppos,pos);

/*  x0=sqrt((*fppos)*(*fppos)+(*(fppos+2))*(*(fppos+2)));
  y0=*(fppos+1); */

  x0=pos[0];
  y0=pos[1];
  a0=tan((*theta-(LAS_TILT))*D_TO_R);
  b0=tan(*phi*D_TO_R);


/*  fprintf(stdout,"x0:%f\n",x0);
  fprintf(stdout,"a0:%f\n",a0);
  fprintf(stdout,"y0:%f\n",y0);
  fprintf(stdout,"b0:%f\n",b0);*/

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
/*  fprintf(stdout,"%2d:x:%f\n",i,(double)(xmat[i].i/1000));
  fprintf(stdout,"%2d:a:%f\n",i,(double)((xmat[i].i%1000)/100));
  fprintf(stdout,"%2d:y:%f\n",i,(double)((xmat[i].i%100)/10));
  fprintf(stdout,"%2d:b:%f\n",i,(double)(xmat[i].i%10));
  fprintf(stdout,"\n\n",(double)(xmat[i].i%10));*/
  }

/*  xc*=1000;
  yc=*(fppos+1);
  ac*=R_TO_D;
  bc=*phi;*/

  xc*=3000;
  yc=*(fppos+1);
  ac*=(R_TO_D/1000.);
  bc=*phi;

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
      SUBROUTINE TRACE(X0,Y0,A0,B0,XC,YC,AC,BC,ICOR)                   
      REAL*4 X0,Y0,A0,B0,XC,YC,AC,BC             
      REAL*4 X00,Y00,A00,B00,RAD,FAC,ACNST             
      DATA RAD /0.01745329/                               
      DATA FAC /4000.0/, ACNST /0.0/                               
      dimension ixmat1(50),iamat1(50),ixmat0(50),iamat0(50),
     -          xmat1(50),amat1(50),xmat0(50),amat0(50)
      common /kmatrx/ixmat1,iamat1,ixmat0,iamat0,xmat1,amat1,xmat0,amat0
c
      IF (ICOR.EQ.0) THEN                                               
        XC=X0                                                           
        YC=Y0                                                           
        AC=A0
        BC=B0
        RETURN                                                          
      ENDIF                                                             
c
      x00=x0
      a00=a0
      y00=y0
      b00=b0
      if (x00.eq.0.) x00=1.e-10
      if (a00.eq.0.) a00=1.e-10
      if (y00.eq.0.) y00=1.e-10
      if (b00.eq.0.) b00=1.e-10
c
      xc=0
      do i=1,50
        if (ixmat1(i).lt.0) goto 10000
          xc=xc+xmat1(i)*x00**int(ixmat1(i)/1000)
     $                  *a00**int((mod(ixmat1(i),1000))/100)
     $      *y00**int((mod(ixmat1(i),100))/10)*b00**mod(ixmat1(i),10)
      enddo
10000 xc=xc*fac
c
      ac=0
      do i=1,50
        if (iamat1(i).lt.0) goto 20000
          ac=ac+amat1(i)*x00**int(iamat1(i)/1000)
     $                  *a00**int((mod(iamat1(i),1000))/100)
     $      *y00**int((mod(iamat1(i),100))/10)*b00**mod(iamat1(i),10)
      enddo
20000 ac=ac/1000./rad-acnst
c
c
      return
      end
C *******************************************************************   

*********************************************************************
 las-matrix
c--akv1=0.2e-4             ! k-value of this matrix
       ixmat1( 1)=         0
       xmat1( 1)= 0.22806E-01
       ixmat1( 2)=      1000
       xmat1( 2)= 0.23392E-03
       ixmat1( 3)=       100
       xmat1( 3)=-0.29214E-01
       ixmat1( 4)=        10
       xmat1( 4)= 0.90449E-05
       ixmat1( 5)=         1
       xmat1( 5)= 0.61528E-02
       ixmat1( 6)=      2000
       xmat1( 6)= 0.65014E-07
       ixmat1( 7)=      1100
       xmat1( 7)=-0.33089E-05
       ixmat1( 8)=      1010
       xmat1( 8)=-0.10777E-07
       ixmat1( 9)=      1001
       xmat1( 9)= 0.11167E-04
       ixmat1(10)=       200
       xmat1(10)= 0.16362E-01
       ixmat1(11)=       110
       xmat1(11)=-0.76469E-05
       ixmat1(12)=       101
       xmat1(12)=-0.14984E-01
       ixmat1(13)=        20
       xmat1(13)= 0.89652E-07
       ixmat1(14)=        11
       xmat1(14)=-0.22450E-04
       ixmat1(15)=         2
       xmat1(15)=-0.10874E-02
       ixmat1(16)=      3000
       xmat1(16)= 0.49676E-11
       ixmat1(17)=      2100
       xmat1(17)=-0.48154E-07
       ixmat1(18)=      2010
       xmat1(18)=-0.44922E-11
       ixmat1(19)=      2001
       xmat1(19)=-0.21891E-08
       ixmat1(20)=      1200
       xmat1(20)=-0.77687E-05
       ixmat1(21)=      1110
       xmat1(21)= 0.61315E-08
       ixmat1(22)=      1101
       xmat1(22)=-0.83385E-05
       ixmat1(23)=      1020
       xmat1(23)= 0.40743E-10
       ixmat1(24)=      1011
       xmat1(24)= 0.20944E-07
       ixmat1(25)=      1002
       xmat1(25)= 0.17309E-05
       ixmat1(26)=       300
       xmat1(26)=-0.48981E-02
       ixmat1(27)=       210
       xmat1(27)= 0.20841E-05
       ixmat1(28)=       201
       xmat1(28)= 0.53219E-02
       ixmat1(29)=       120
       xmat1(29)= 0.32648E-08
       ixmat1(30)=       111
       xmat1(30)= 0.11717E-04
       ixmat1(31)=       102
       xmat1(31)=-0.73120E-03
       ixmat1(32)=        21
       xmat1(32)= 0.12566E-06
       ixmat1(33)=        12
       xmat1(33)=-0.52859E-04
       ixmat1(34)=         3
       xmat1(34)= 0.35165E-01
       ixmat1(35)=      4000
       xmat1(35)=-0.35735E-13
       ixmat1(36)=       400
       xmat1(36)=-0.13701E-03
       ixmat1(37)=        40
       xmat1(37)=-0.66081E-11
       ixmat1(38)=         4
       xmat1(38)= 0.10286E-01
       ixmat1(39)=      3100
       xmat1(39)= 0.30037E-10
       ixmat1(40)=      2200
       xmat1(40)=-0.32596E-08
       ixmat1(41)=      1300
       xmat1(41)= 0.75084E-05
       ixmat1(42)=        -1
       iamat1( 1)=         0
       amat1( 1)=-0.90882E+03
       iamat1( 2)=      1000
       amat1( 2)=-0.98402E+00
       iamat1( 3)=       100
       amat1( 3)= 0.21078E+04
       iamat1( 4)=        10
       amat1( 4)=-0.81389E-02
       iamat1( 5)=         1
       amat1( 5)=-0.15176E+03
       iamat1( 6)=      2000
       amat1( 6)=-0.47535E-03
       iamat1( 7)=      1100
       amat1( 7)= 0.20539E+01
       iamat1( 8)=      1010
       amat1( 8)= 0.82019E-05
       iamat1( 9)=      1001
       amat1( 9)=-0.10719E+00
       iamat1(10)=       200
       amat1(10)=-0.20867E+04
       iamat1(11)=       110
       amat1(11)=-0.39076E-02
       iamat1(12)=       101
       amat1(12)= 0.23063E+03
       iamat1(13)=        20
       amat1(13)= 0.17441E-02
       iamat1(14)=        11
       amat1(14)= 0.17144E+00
       iamat1(15)=         2
       amat1(15)= 0.46881E+02
       iamat1(16)=      3000
       amat1(16)=-0.12898E-06
       iamat1(17)=      2100
       amat1(17)= 0.72953E-03
       iamat1(18)=      2010
       amat1(18)=-0.50616E-09
       iamat1(19)=      2001
       amat1(19)= 0.72687E-05
       iamat1(20)=      1200
       amat1(20)=-0.15317E+01
       iamat1(21)=      1110
       amat1(21)=-0.72434E-05
       iamat1(22)=      1101
       amat1(22)= 0.76453E-01
       iamat1(23)=      1020
       amat1(23)= 0.38152E-06
       iamat1(24)=      1011
       amat1(24)= 0.44978E-04
       iamat1(25)=      1002
       amat1(25)= 0.41038E-01
       iamat1(26)=       300
       amat1(26)= 0.10045E+04
       iamat1(27)=       210
       amat1(27)= 0.33079E-02
       iamat1(28)=       201
       amat1(28)=-0.76794E+02
       iamat1(29)=       120
       amat1(29)=-0.94281E-03
       iamat1(30)=       111
       amat1(30)=-0.38992E-01
       iamat1(31)=       102
       amat1(31)=-0.10189E+01
       iamat1(32)=        21
       amat1(32)=-0.62901E-03
       iamat1(33)=        12
       amat1(33)=-0.92175E-01
       iamat1(34)=         3
       amat1(34)=-0.21562E+03
       iamat1(35)=      4000
       amat1(35)=-0.13113E-10
       iamat1(36)=       400
       amat1(36)=-0.18417E+03
       iamat1(37)=        40
       amat1(37)=-0.23016E-07
       iamat1(38)=         4
       amat1(38)=-0.30098E+03
       iamat1(39)=      3100
       amat1(39)= 0.97679E-07
       iamat1(40)=      2200
       amat1(40)=-0.27604E-03
       iamat1(41)=      1300
       amat1(41)= 0.38034E+00
       iamat1(42)=        -1
****************************************************************************/


