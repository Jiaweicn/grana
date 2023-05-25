/* grmat.c 08/12/30 for E308 */

#include <stdio.h>
#include <math.h>

#include "ana10.h"
#include "config.h"

#define d2r PI/180.0
#define r2d 180.0/PI

#define N_ELE_X 13
#define N_ELE_A 5
#define N_ELE_Y 5
#define N_ELE_B 17

extern struct runparm grcr;
extern struct vdcdata grfx;

void grmat(double *fppos,double *colpos,double *theta,double *phi){
  double x0,xc=0,y0,yc=0,a0,ac=0,b0,bc=0;
  double tmpa0,tmpy0,tmpb0;

  struct mat {
    int i;
    double element;
  };

#if 0
  static struct mat xmat[N_ELE_X]={
    {0x0000,   -1.9209089e-01},    {0x1000,   -7.1072374e-03},
    {0x0010,    6.2985574e-02},    {0x0020,   -7.3395426e-03},
    {0x2000,   -1.9095304e-06},    {0x1010,    2.0837271e-04},
    {0x1020,   -1.5184985e-05},    {0x2010,    2.8345402e-07},
    {0x2020,   -4.9226797e-09},    {0x0100,   -1.7179483e-04},
    {0x0200,    3.3636539e-05},    {0x1100,   -7.4634216e-08},
    {0x1200,    2.3670998e-08},    {0x2100,    7.7535580e-10},
    {0x2200,   -1.8856727e-10},    {0x3000,   -1.4085213e-09}
  };
#endif
#if 1  /* With x**2 term */
  static struct mat xmat[N_ELE_X]={
    {0x0000,      -9.5154797e-02},
    {0x1000,      -6.6070028e-03},
    {0x0010,       3.3092378e-02},
    {0x0020,      -5.9067598e-03},
    {0x1010,       5.5466668e-05},
    {0x1020,      -4.9281264e-06},
    {0x2010,       8.2061311e-08},
    {0x2020,      -1.0458680e-08},
    {0x3010,       2.3429897e-10},
    {0x3020,      -2.5898813e-10},
    {0x4010,       3.8792685e-13},
    {0x4020,      -7.1805979e-13},
    {0x2000,      -6.6634147e-07}
  };
#endif

#if 0  /* Without x**2 term */
  static struct mat xmat[N_ELE_X]={
    {0x0000,   -1.1331700e-01},
    {0x1000,   -6.5275122e-03},
    {0x0010,    3.3033719e-02},
    {0x0020,    5.6904999e-04},
    {0x1010,    5.5856524e-05},
    {0x1020,   -3.2366467e-05},
    {0x2010,    8.4823941e-08},
    {0x2020,   -2.5409750e-07},
    {0x3010,    2.1440602e-10},
    {0x3020,   -2.8765861e-10},
    {0x4010,    3.0899121e-13},
    {0x4020,   -6.7264651e-13},
    {0x2000,    0.0000000}};  
#endif

  static struct mat amat[N_ELE_A]={
    {0x0010,   -4.4766176e-01},
    {0x0020,   -4.8635801e-04},
    {0x1010,   -2.7509397e-05},
    {0x1020,   -3.9445223e-06},
    {0x2010,    3.5880244e-08}
  };

#if 0
  static struct mat ymat[N_ELE_Y]={
    {0x0000,    -2.8965083e+00},    {0x0001,     2.4039317e+01},
    {0x0010,    -1.6032874e+00},    {0x0011,     8.1903926e+00},
    {0x0020,     4.2124272e-02},    {0x0021,    -1.4897976e-02},
    {0x1000,     2.0038815e-02},    {0x1001,    -7.4466939e-02},
    {0x1010,     8.6991528e-03},    {0x1011,    -4.6778923e-02},
    {0x1020,    -3.7698000e-04},    {0x1021,     2.7812769e-03},
    {0x2000,    -6.8395164e-05},    {0x2001,     3.5946477e-04},
    {0x2010,    -5.5070886e-06},    {0x2011,     4.9519843e-06},
    {0x2020,    -3.4696067e-07},    {0x2021,    -1.3759220e-06}
  };
#endif

  static struct mat ymat[N_ELE_Y]={
    {0x0100,     1.0000000e+00},
    {0x0000,    -1.8167931e+00},
    {0x1000,    -2.9496111e-03},
    {0x2000,    -2.4615192e-06},
    {0x3000,     4.2557011e-08}
  };

  static struct mat bmat[N_ELE_B]={
    {0x0100,  1.8912305e-01},    {0x0200, -3.3211560e-06},
    {0x0110, -2.0117353e-02},    {0x0210, -4.8006718e-04},
    {0x0120,  3.5396679e-03},    {0x1100,  2.6046141e-04},
    {0x1200, -4.8867711e-07},    {0x2100,  1.8598118e-07},
    {0x2200, -2.2168812e-09},    {0x1110,  5.0856034e-05},
    {0x1120, -3.0005019e-05},    {0x2110,  2.0811024e-07},
    {0x1210, -1.8952720e-06},    {0x2210, -4.2403755e-09},
    {0x2120, -9.9225146e-08},    {0x1220,  1.4517040e-08},
    {0x2220,  1.0628261e-09}
  };


/** Unit and convention  *********************
   x0:[mm]: distance from central ray on focal plane
   y0:[mm]: distance from central ray on focal plane
   a0:[deg]: Horizontal angle on folcal plane
   b0:[deg]: Vertical angle on focal plane

#define VDC_TILT 45.0
#define VDCPOS_FX {{573.0,0,0},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_FU {{602.9,0,20},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_FV {{599.1,0,40},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RX {{323.0,0,250},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_RU {{352.7,0,270},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RV {{349.3,0,290},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
#define GR_PL10 {1,0,1,190.0}  focal plane 

   xc:[%]:  Momentum of the analyzed particle. 100%=>Run2131 (974.704 mT)
   yc:[mm]: Beam spot position in the vertical direction.
   ac:[deg]: Horizontal scattering angle.
   bc:[deg]: Vertical scattering angle.


*************************************************/

  int i;
  double pos[3];
  double y_tgt;
  /*  rotate_y(-grfx.tilt_s,grfx.tilt_c,fppos,pos);*/

  x0=fppos[0];
  y0=fppos[1];
  a0=theta[0];
  b0=phi[0];

#if 0
  fprintf(stdout,"x0:%f\n",x0);
  fprintf(stdout,"a0:%f\n",a0);
  fprintf(stdout,"y0:%f\n",y0);
  fprintf(stdout,"b0:%f\n",b0);
#endif
  tmpa0=a0-2.0651893e+00-4.6694977e-03*x0-7.8890315e-07*x0*x0;
  for(i=0;i<N_ELE_X;i++){
    xc+=(xmat[i].element
	 *pow(x0,(double)(xmat[i].i>>12))        
	 *pow(y0,(double)((xmat[i].i>>8)&0xf))   
	 *pow(tmpa0,(double)((xmat[i].i>>4)&0xf))   
	 *pow(b0,(double)(xmat[i].i&0xf)));
    /*
      fprintf(stdout,"%2d %04x %d%d%d%d %10.2f %7.2f %7.2f %7.2f %7.2f\n",
      i,xmat[i].i,
      (xmat[i].i>>12),
      ((xmat[i].i>>8)&0xf),
      ((xmat[i].i>>4)&0xf),
      (xmat[i].i&0xf),
      xmat[i].element,
      pow(x0,(double)(xmat[i].i>>12)),
      pow(a0,(double)((xmat[i].i>>8)&0xf)),
      pow(y0,(double)((xmat[i].i>>4)&0xf)),  
      pow(b0,(double)(xmat[i].i&0xf)));*/
  }


  for(i=0;i<N_ELE_A;i++){
    ac+=(amat[i].element
		 *pow(x0,(double)(amat[i].i>>12))
		 *pow(y0,(double)((amat[i].i>>8)&0xf))
		 *pow(tmpa0,(double)((amat[i].i>>4)&0xf))
		 *pow(b0,(double)(amat[i].i&0xf)));
  }
  /**  Added by T. Kawabata on Jan 04, 2007 **/
  /**  to correct the kinematical effect on 197Au(a,a). **/
  /*  xc+=((-3.40703E-04*ac-4.10467E-03)*ac);*/
  /**  to correct the kinematical effect on 58Ni(a,a). **/
  xc+=(-0.19674e-1*ac-0.12051e-2*ac*ac);

  for(i=0;i<N_ELE_Y;i++){
    yc+=(ymat[i].element
	 *pow(x0,(double)(ymat[i].i>>12))
	 *pow(y0,(double)((ymat[i].i>>8)&0xf))
	 *pow(a0,(double)((ymat[i].i>>4)&0xf))
	 *pow(b0,(double)(ymat[i].i&0xf)));
  }
  tmpy0=yc+(-5.1651-.18535*xc-.11169*xc*xc-.13320*xc*xc*xc)*grcr.ytgt/4.5;
  /*tmpy0=yc;*/
  tmpb0=b0+(-9.1838496e-02+2.9112199e-04*x0+6.0204302e-08*x0*x0);


  /*  b0 = b0-(-0.2899*y_tgt+0.06874*y_tgt*xc-0.006797*y_tgt*xc*xc-0.001244*y_tgt*y_tgt*xc*xc);*/

  for(i=0;i<N_ELE_B;i++){
    bc+=(bmat[i].element
		 *pow(x0,(double)(bmat[i].i>>12))
		 *pow(tmpy0,(double)((bmat[i].i>>8)&0xf))
		 *pow(tmpa0,(double)((bmat[i].i>>4)&0xf))
		 *pow(tmpb0,(double)(bmat[i].i&0xf)));
  }
  bc+=0.19623424*grcr.ytgt/4.5*ac;

#if 0
  fprintf(stdout,"xc:%f\n",xc);
  fprintf(stdout,"ac:%f\n",ac);
  fprintf(stdout,"yc:%f\n",yc);
  fprintf(stdout,"bc:%f\n",bc);
  fprintf(stdout,"\n\n");
#endif
  *colpos=xc;
  *(colpos+1)=yc;
  *(theta+1)=ac;
  /*  *(phi+1)=bc-(-0.2899*y_tgt+0.06874*y_tgt*xc-0.006797*y_tgt*xc*xc-0.001244*y_tgt*y_tgt*xc*xc);*/
  *(phi+1)=bc;
}

