/************** config.h*********************/
/***** for e207(0pol-SDR) experiment **********/
/* 00/07/08  added compile options for each experiment */
/* 00/06/23  added 0deg parameters   */
/* 00/05/15  added macro MTAR define target mass */
/* 00/05/12  added virtual projection prlanes */
/** modified 00/03/07  for DSR- **/
/** 00/01/28 ***/
/* for ana_v1.5 or later */


#define _GRNORMAL 1

#if 0
#define _E137DEC99_P 1
#define _E1370DEG 1
#define _E137APR00 1
#define _E137JUN00 1
#define _E137DEC99_N 1
#endif

#ifdef _E137DEC99_P
#define _E137DEC99 1
#endif

#ifdef _E137DEC99_N
#define _E137DEC99 1
#endif

#ifdef _E137JUN00
#define _E137APR00 1
#endif

/* virtual projection plane {normal vector,z} for GR */

/*Plane 11 must be downstream of plane 10. (GR_PL11_z > GR_PL_10_z) */

#define NPL 11 /* number of virtual plane */

#define GR_PL1  {0,0,1,1210.5} /* MWPC_X1 */
#define GR_PL2  {0,0,1,1466.5} /* MWPC_X2 */
#define GR_PL3  {0,0,1,1744.0} /* Carbon Block */
#define GR_PL4  {0,0,1,1873.0} /* MWPC_U3 */
#define GR_PL5  {0,0,1,1885.0} /* MWPC_V3 */
#define GR_PL6  {0,0,1,2220.5} /* MWPC_X4 */
#define GR_PL7  {0,0,1,2232.5} /* MWPC_U4 */
#define GR_PL8  {0,0,1,2244.5} /* MWPC_V4 */
#define GR_PL9  {0,0,1,2557.5} /* Hodoscope X */

/* default */
#ifdef _GRNORMAL  /* 45.0deg */
/*#define GR_PL10 {1,0,1,150.0}*/  /* focal plane */
#define GR_PL10 {1,0,1,180.0}  /* focal plane */
#define GR_PL11 {1,0,1,260.0}  /* for calculate scattering angle */
#define GR_XMAG -0.4376
#define GR_YMAG  5.98  
#endif


/* 0deg E137Apr00 */
#ifdef _E137APR00 /* 51.8deg */
#define GR_PL10 {0.7855,0,0.6188,20.0}  /* focal plane */
#define GR_PL11 {0.7855,0,0.6188,60.0}  /* for calculate scattering angle */
#define GR_XMAG -0.437
#define GR_YMAG  5.98  
#endif


/* DSR+ */
#ifdef _E137DEC99_P  /* 46.6deg */
#define GR_PL10 {0.724,0,0.690,50.0}  /* DSR + */
#define GR_PL11 {0.724,0,0.690,100.0}
#define GR_XMAG -0.4477
#define GR_YMAG  5.05   
#endif

/* DSR- */
#ifdef _E137DEC99_N  /* 32.48deg */
#define GR_PL10 {0.5369,0,0.8436,-40.0}  /* DSR - */
#define GR_PL11 {0.5369,0,0.8436,100.0}
#define GR_XMAG -0.441
#define GR_YMAG  5.57   
#endif

/* virtual projection plane {normal vector,z} for LAS */
#define LAS_PL1  {0,0,1,0.0}    /* LAS_X1 */
#define LAS_PL2  {0,0,1,20.0}   /* LAS_U1 */
#define LAS_PL3  {0,0,1,40.0}   /* LAS_V1 */
#define LAS_PL4  {0,0,1,164.0}  /* LAS_X2 */
#define LAS_PL5  {0,0,1,184.0}  /* LAS_U2 */
#define LAS_PL6  {0,0,1,204.0}  /* LAS_V2 */
#define LAS_PL7  {0,0,1,-100.0} /*  */
#define LAS_PL8  {0,0,1,300.0}  /*  */
#define LAS_PL9  {0,0,1,500.0}  /*  */
#define LAS_PL10 {-0.80942715,0,0.58722031,0.0}  /* focal plane */
#define LAS_PL11 {-0.80942715,0,0.58722031,2000.0} 
                             /* for calculate scattering angle */

/** for 3377 *********************************************************/
#define VDC_DX 0.7453559924999300
#define VDC_DY 0.6666666666666667


/* At the moment,
  wire-direction vectors of parallel planes 
  (for example FX and RX, FU and RU) must indicate same direction */

/* {{first wire position},{wire dicplacement},{wire direction},tilt-angle} */

/* for e137(0pol-SDR) Apr00 */
/* This setting needs new raytrace.c routine. */

#ifdef _E137APR00
#define VDC_TILT 44.085 
/****************************************************************/
#define VDCPOS_FX {{675.3,0,30},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_FU {{704.3,0,50},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_FV {{750.6,0,70},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RX {{425.3,0.23,280},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_RU {{454.3,0.23,300},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RV {{500.8,0.23,310},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
/****************************************************************/
#endif


/* E137 99dec (DSR) */
#ifdef _E137DEC99
#define VDC_TILT 40.13  
/****************************************************************/
#define VDCPOS_FX {{573.0,0,0},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_FU {{602.9,0,20},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_FV {{599.1,0,40},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RX {{323.0,0,250},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_RU {{352.7,0,270},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RV {{349.3,0,290},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
/****************************************************************/
#endif


/* E114 99may (Normal) */
#ifdef _GRNORMAL
#if 1
/* in underforcus mode Nov. 6, 2010 */
#define VDC_TILT 44.7
#endif
#if 0
/* in normal mode Nov. 6, 2010 */
#define VDC_TILT 45.5
#endif
/****************************************************************/
#define VDCPOS_FX {{573.0,0,0},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_FU {{602.9,0,20},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_FV {{599.1,0,40},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RX {{323.0,0,250},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_RU {{352.7,0,270},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RV {{349.3,0,290},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
/****************************************************************/
#endif


/* E108 98may (0pol-2) */
#if 0
#define VDC_TILT 43.885 
/* #define VDC_TILT 43.745 */
/****************************************************************/
#define VDCPOS_FX {{724.5,0,0},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_FU {{754.4,0,20},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_FV {{750.6,0,40},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RX {{474.5,0,250},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_RU {{504.2,0,270},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RV {{500.8,0,290},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
/****************************************************************/
#endif


/* E96 97jul (0pol) */
#if 0
#define VDC_TILT 43.51 
/****************************************************************/
#define VDCPOS_FX {{726.0,0,0},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_FU {{755.9,0,20},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_FV {{752.1,0,40},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RX {{476.0,0,250},{-6,0,0},{0,1,0},VDC_TILT}
#define VDCPOS_RU {{505.7,0,270},{-6,0,0},{VDC_DX,VDC_DY,0},VDC_TILT}
#define VDCPOS_RV {{502.3,0,290},{-6,0,0},{-VDC_DX,VDC_DY,0},VDC_TILT}
/****************************************************************/
#endif


/** for MWPC_PCOS ****************************************************/
#define PC_TILT 0
#define PC_DX 1.41421356237309 

/* At the moment,
  wire-direction vectors of parallel planes 
  (for example FX and RX, FU and RU) must indicate same direction */

/* {{first wire position},{wire dicplacement},{wire direction},tilt-angle} */


#ifdef _GRNORMAL
/* from data */
#define PCPOS_X1 {{467.79, 0,1210.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X2 {{467.59, 0,1466.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X3 {{698.42, -1.5,1861.0},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U3 {{-908.26,-1.5,1873.0},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V3 {{899.10, -1.5,1885.0},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#define PCPOS_X4 {{699.62, -1.25,2221.0},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U4 {{-997.57,-1.25,2233.0},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V4 {{990.81, -1.25,2245.0},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#endif

/*************************************************/
/* for e137(0pol-SDR) 00Apr */
/* This setting needs new raytrace.c routine. */
#ifdef _E137APR00
#define PCPOS_X1 {{485.0, 0,1210.5},{-0.99,0,0}, {0,1,0},PC_TILT}
#define PCPOS_X2 {{485.0, 0,1466.5},{-0.99,0,0}, {0,1,0},PC_TILT}
#define PCPOS_X3 {{1123.0,2.28,1861.0},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U3 {{-483.7,2.28,1873.0},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V3 {{1323.7,2.28,1885.0},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#define PCPOS_X4 {{1118.2,2.18,2221.1},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U4 {{-579.0,2.18,2233.1},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V4 {{1409.4,2.18,2245.1},{-PC_DX,0,0},{1,-1,0},PC_TILT}  
#endif
/*************************************************/

/*************************************************/
/* for e137(0pol-SDR) 99Dec */
#ifdef _E137DEC99
/* from data */
#define PCPOS_X1 {{467.79, 0,1210.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X2 {{467.59, 0,1466.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X3 {{698.42, -1.5,1861.0},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U3 {{-908.26,-1.5,1873.0},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V3 {{899.10, -1.5,1885.0},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#define PCPOS_X4 {{699.62, -1.25,2221.0},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U4 {{-997.57,-1.25,2233.0},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V4 {{990.81, -1.25,2245.0},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#endif
/*************************************************/


/*************************************************/
/* for e108(0pol-2) may97(from data) */
/* from data */
#if 0
#define PCPOS_X1 {{471.2, 0,1210.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X2 {{472.2, 0,1466.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X3 {{1138.0,0,1861.0},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U3 {{-470.7,0,1873.0},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V3 {{1340.7,0,1885.0},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#define PCPOS_X4 {{1131.3,0,2220.5},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U4 {{-566.4,0,2232.5},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V4 {{1423.1,0,2244.5},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#endif
/*************************************************/

/*************************************************/
/* for e96(0pol) Jul97 */
#if 0
#define PCPOS_X1 {{490.4, 0,1210.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X2 {{490.2, 0,1466.5},{-1,0,0},    {0,1,0},PC_TILT}
#define PCPOS_X3 {{1130.0,0,1861.0},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U3 {{-478.7,0,1873.0},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V3 {{1332.7,0,1885.0},{-PC_DX,0,0},{1,-1,0},PC_TILT}
#define PCPOS_X4 {{1123.3,0,2220.5},{-2,0,0},    {0,1,0},PC_TILT}
#define PCPOS_U4 {{-574.4,0,2232.5},{PC_DX,0,0}, {1,1,0},PC_TILT}
#define PCPOS_V4 {{1415.1,0,2244.5},{-PC_DX,0,0},{1,-1,0},PC_TILT} 
#endif
/*************************************************/

#define LAS_TILT -54.04
#define LAS_DX 0.51503807
#define LAS_DY 0.8571673

#define LASVDC_FX {{-757.76,0,0},{6,0,0},{0,1,0},LAS_TILT}
#define LASVDC_FU {{-825.76,0,20},{6.99980,0,0},{LAS_DX,LAS_DY,0},LAS_TILT}
#define LASVDC_FV {{-805.76,0,40},{6.9998,0,0},{-LAS_DX,LAS_DY,0},LAS_TILT}
#define LASVDC_RX {{-508.36,0,164},{6,0,0},{0,1,0},LAS_TILT}
#define LASVDC_RU {{-576.36,0,184},{6.9998,0,0},{LAS_DX,LAS_DY,0},LAS_TILT}
#define LASVDC_RV {{-556.36,0,204},{6.9998,0,0},{-LAS_DX,LAS_DY,0},LAS_TILT}


/** Kinematics (GR) **************************************************/
#ifndef _KINE
#include "kinema.h"
#endif

/*********************************************************************/

/** for various cuts (GR) ******************************************/
#if 1
#define THETAX_C_L  -0.8   /* lower limit of scattering angle(X)*/
#define THETAX_C_H   0.8   /* upper limit of scattering angle(X)*/
#define THETAY_C_L  -7.5   /* lower limit of scattering angle(Y)*/
#define THETAY_C_H  7.5    /* upper limit of scattering angle(Y)*/
#endif

#ifndef _E137SINGL 
#define Y_C_PEAK_L -2.5  /* lower limit of y-cut (peak) */
#define Y_C_PEAK_H  2.5   /* upper limit of y-cut (peak) */
#define Y_C_BACK1_L -4.5  /* lower limit of y-cut (back ground1) */
#define Y_C_BACK1_H -2.5   /* upper limit of y-cut (back ground1) */
#define Y_C_BACK2_L  2.5    /* lower limit of y-cut (back ground2) */
#define Y_C_BACK2_H  4.5   /* upper limit of y-cut (back ground2) */
#endif

#define FPP_THETA_L 5.0  /* lower limit of fpp scatt. angle */
#define FPP_THETA_H 20.0 /* upper limit of fpp scatt. angle */
#define FPP_PHI_L 23.2  /* lower limit of fpp scatt. angle */
#define FPP_PHI_H 156.8   /* upper limit of fpp scatt. angle */
#define FPP_VER_L 1630    /* lower limit of vertex position-z */
#define FPP_VER_H 1860    /* lower limit of vertex position-z */

#ifdef _GRNORMAL
#define P_ID_L  135    /* lower limit of particle ID */
#define P_ID_H  400
#define P_ID_L2  150    /* lower limit of particle ID 2*/
#define P_ID_H2  500    /* upper limit of particle ID 2 */
#endif

#define RFC1 642
#define RFC2 1653
#define RFCW 100

/** for FPP ****************************************************/
#define FPP_N {0.,1.,0.} 
/* if you change this parameter, 
   you MUST change the way of calculation of FPP scatt. angle. */

/** for various cuts (LAS) ******************************************/
#define PID_LASL1  90     /* lower limit of particle ID */
#define PID_LASH1  1500    /* upper limit of particle ID */
#define PID_LASL2  100     /* lower limit of particle ID */
#define PID_LASH2  500     /* upper limit of particle ID */
#define PID_LASL3  150      /* lower limit of particle ID */
#define PID_LASH3  1000     /* upper limit of particle ID */


