/************** ana10.h*********************/
/* for ana_v1.64 or later 07/01/17 */
/* 00/05/12 removed virtual projection planes. */
/* 00/12/07 Change FHSIZE to FIHSIZE to compile on AIX */
/* 05/10/03 Add LASFVTBL and LASRVTBL. */
/* 07/01/17 Add vdcdata.svec[3] and vdcdata.plane[4]. */
/*          Change data type of vdcdata.posid */
/* 08/08/24 Add a description on Reserved Planes. */
/*          GR_FR_R, GR_RE_R, LAS_FR_R, GR_RE_R.   */
/* 09/01/17 Define a function "t2dl" which converts */
/*                    time to drift length.                   */
/* 09/04/02 Add a member of parm grpid1[2]  */



#ifndef _H_STDIO
#include <stdio.h>
#endif

#ifndef PI
#define PI 3.14159265359
#endif

#define D_TO_R (PI/180)
#define R_TO_D (180/PI)

/* Event ID definition */
/* The following definitions of event ID are smaller than 
   TAMI-definitions by one. */
#define SPIN_DOWN         1
#define SPIN_UP           2
#define HATA_OUT_CV       3
#define HATA_OUT_WN       4
#define GR_FP_EVENT       5
#define POLAR_EVENT       6
#define LAS_FP_EVENT      7
#define COIN              8
#define SEC_TRIG_EVENT   10
#define FPP_EVENT        11
#define SEC_TRIG_ACCEPT  12
#define SEC_TRIG_REJECT  13
#define SEC_TRIG_BAD     14
#define BLOCK_END        15


/* Region ID definition */

#define IRREGAL           0
#define INPUT_REG         2
#define ADC               3
#define TDC               4
#define SSD_3351          5
#define SCALER            6
#define VDC_3377          7
#define MWPC_PCOS         10
#define ADC_LAS           11
#define TDC_LAS           12
#define FERA              13
#define FERET             14
#define CHECK_SUM         15

/* Buffer Size */
#ifndef _BLP2
#define BUFF_SIZE  65536
/*#define BUFF_SIZE  49152*/
#else
#define BUFF_SIZE  16384
#endif

#define BUFF_WORD (BUFF_SIZE / sizeof(short))


/*Block header size*/
#define BHSIZE 6

/*EVENT header size*/
#define EHSIZE 6

/*Number of field in one event*/
#define N_FIELD 1

/*Field header size*/
#define FIHSIZE 4


/* for chamber configuration */
struct chamber {
  double position[3];    /* first wire position */
  double displace[3];    /* next wire displacement */
  double wiredirec[3];   /* wire direction */  
  double tilt;           /* chamber tilt angle relative to the z-axis */
};
#define POSE 5               /* Buffer size for positioning error */
#define RAYE 5               /* Buffer size for ray tracing error */

#define GR_FERA    0x00
#define LAS_FERA   0x01
#define SSD_FERA   0x03

/** for scaler *******************************************************/
#define N_SCA 4        /* number of scaler modules */
#define SC_MAX 0xffff   /* Max value in one block */
/** for 3377 *********************************************************/
#define GR_FR_X 0x8100    /* chamber ID */
#define GR_FR_R 0x8110
#define GR_FR_U 0x8120
#define GR_FR_V 0x8130
#define GR_RE_X 0x8140
#define GR_RE_R 0x8150
#define GR_RE_U 0x8160
#define GR_RE_V 0x8170
#define LAS_FR_X   0x8180
#define LAS_FR_R   0x8190
#define LAS_FR_U   0x81a0
#define LAS_FR_V   0x81b0
#define LAS_RE_X   0x81c0
#define LAS_RE_R   0x81d0
#define LAS_RE_U   0x81e0
#define LAS_RE_V   0x81f0


#define VDCEVE_L 310          /* Maximum number of wire hits */
#define FXTBL "grfx.tbl"      /* filename of timetable */
#define FUTBL "grfu.tbl"
#define RXTBL "grrx.tbl"
#define RUTBL "grru.tbl"
#define LASFXTBL "lasfx.tbl"      /* filename of timetable */
#define LASFUTBL "lasfu.tbl"
#define LASFVTBL "lasfv.tbl"
#define LASRXTBL "lasrx.tbl"
#define LASRUTBL "lasru.tbl"
#define LASRVTBL "lasrv.tbl"
#define L_3377 1024           /* TDC data length of 3377 */
#define VDCNCL 32            /* Maximum number of clusters */
#define DMAX 1               /* Maximum drift length */

struct vdcdata {
  char label[10];            /* chmaber name */
  short channel[VDCEVE_L];
  short time[VDCEVE_L];
  int wire;               /* number of hit wires in each chamber */
  double table[L_3377];   /* table to connect drift-time and drift-length */
  double derr[L_3377];   /* table to connect drift-time and drift-length */
  int time_l,time_h;      /* drift_start_ch and drift_end_ch */
  int nclu;               /* number of clusters per one chamber */
  int nhit[VDCNCL];       /* number of hits wire per one cluster */ 
  int cclu[VDCNCL];       /* first wire in each cluster */ 
  int nlclu;          /* number of large clusters (nhit>2) per one chamber */
  int nlhit[VDCNCL];   /* number of hits wire per one large cluster */ 
  int clclu[VDCNCL];       /* first wire in each large cluster */ 
  unsigned int posid;      /* error in positioning */
  int good;               /* indicate  good event */
  unsigned int eff;       /* for efficiency check */
  int min;                /* center of cluster decided by TDC data*/
  double pos[3];          /* vdc positon */
  double rpos[3];         /* vdc positon after rotation */
  double dis;        /* distance between lower wire and particle-hit point */
  double dis2;       /* distance decided by two wires except "min"-wire*/
  double kai;        /* kaisqure in least kaisqure fitting */
  double res[2];     /* for check resolution */
                     /* [0]..drift length for min wire */
                     /* [1]..arithmetical mean over the two wires */
  short worst;  /* worst wire in first kaisqure fitting */
  double tilt_c,tilt_s;    /* chamber tilt angle */
  double rayang;    /* ray angle decided by single plane in degree */
  double fppos;  /* position on focal plane decide by single plane */
  double svec[3];   /* ray vector determined by a single plane */
  double plane[4];  /* ray plane determined by a single plane */
};

void sort3377(short channel[],short time[],int k,char *label);
int go_down(short *time,int numofwire ,int currentwire);
int go_up(short *time,int numofwire ,int currentwire);



/** for MWPC_PCOS ****************************************************/

#define MWPC_X1 0x0000
#define MWPC_X2 0x0800
#define MWPC_X3 0x1000
#define MWPC_U3 0x3000
#define MWPC_V3 0x5000
#define MWPC_X4 0x1800
#define MWPC_U4 0x3800
#define MWPC_V4 0x5800
#define PCEVE_L 550

struct mwpcdata {
  char label[8];              /* chamber name */
  short channel[PCEVE_L];     /* center wire of the cluster */
  short width[PCEVE_L];       /* width of the cluster */
  short halfbit[PCEVE_L];
  short pos[PCEVE_L];         /* position in each plane */
  short hberror;              /* flag for halfbit errror */
  int wire;                   /* number of hit wires in each chamber */
  short posid[POSE];          /* error in positioning  */
  int good;                   /* indicate good event */
  unsigned int eff;           /* for efficiency check */
  double posv[3];          /* position vector before acounting tilt angle */
  double rposv[3];         /* position vector after acounting tilt angle */
  double tilt_c,tilt_s;       /* chamber tilt angle */
};


/** for SSD_Ball  ****************************************************/
#define N_SSD 40  /* number of SSD */

struct ssddata {
  unsigned short adc[N_SSD]; /* 3351 data of SSD */          
  int adc_ch[N_SSD];             /* 3351 hit channel */          
  int k_adc;                     /* number of 3351 hit channel */
  unsigned short tdc[N_SSD]; /* FERET data of SSD */          
  int k_tdc;                     /* FERET hit channel */          
  int tdc_ch[N_SSD];             /* number of FERET hit channel */

  double adcc[N_SSD];             /* 3351 data in order of array */          
  int adcc_ch[N_SSD];             /* 3351 hit channel */          
  double tdcc[N_SSD];             /* 3351 data in order of array */          
  int tdcc_ch[N_SSD];             /* 3351 hit channel */          

  double tof[N_SSD];    /* Time of flight */
};
  
int event_id();
int input_reg();
int adcread();
int tdcread();
int fera();
int feret();
int vdc_3377();
int mwpc_pcos();
int camac_scaler();
int ssd_3351();

void book1d(int histid,double data);
void book1(int histid,float data);
void book2d(int histid,double datax,double datay);



/* vdccalib.c */
void vdc_calb(int fd);
void tableread(char *s,struct vdcdata *vdc);
void tableread2(int n,double *a,struct vdcdata *vdc);
void fopenerror(char *s);

/* raytrace.c */
void makeplane(double **plane);
void plane_eq(double *rayvec,double *wiredire,double *point,double *eq);
void rotate_y(double sin,double cos,double *point,double *rpoint);
void fpp_rotate_x(double theta_x,double *point,double *rpoint);
void fpp_rotate_y(double theta_y,double *point,double *rpoint);
int calcpoint(double *route_a,double *route_b,double *plain,double *point);
int calcvertex(double a[2][3],double *answer);

/* vdc3377_v4.c */
void vdcread(struct vdcdata *vdc,short channel,short time);
void vdcsort(struct vdcdata *vdc);
void sort3377(short channel[],short time[],int k,char *label);
struct tnode *tree(struct tnode *p,short ch,short tm);
void treep(struct tnode *p,short channel[],short time[],int *i);
int vdcana(struct vdcdata *vdc,struct chamber *cham);
double leastsqrt(struct vdcdata *vdc,int nhit,int cclu);
int wirecheck(struct vdcdata *vdc,int nhit,int fwire);
int vdc_calcpoint(struct vdcdata *vdc,struct chamber *cham,
		     int nclu,int *hit,int *cclu);
void bookvdc(struct vdcdata *vdc, int histid);
double simplefp(struct vdcdata *vdc,struct chamber *cham);
double t2dl(struct vdcdata *vdc, int iwire, int iflag);

/* vdceffv2.c */

void simp_pl(double *fixplane,struct vdcdata *vdc,struct chamber *cham);
void calcfpsimp2(double *planex,double *planeu,double *fppos,double *theta);
void calclassimp2(double *planex,double *planeu,double *fppos,double *theta);
double corsimp(double *fppos, double *fpang,double *ex,double *scang);

/* mwpcpocs2 */
void pcread(struct mwpcdata *,unsigned short, short);
int mwpcana(struct mwpcdata *,struct chamber *);

/* mwpceff.c */
void calcpcsimp(struct mwpcdata *mwpcu, struct mwpcdata *mwpcv,
		struct chamber *chamu, struct chamber *chamv,
		double *rayvdc, double *vertex,double *ang);

/* grmat.c */
void grmat(double *fppos,double *colpos,double *theta,double *phi);

/* lasmat.c */
void lasmat(double *fppos,double *colpos,double *theta,double *phi);

/** for FPP ****************************************************/
#define FPP_N {0.,1.,0.} 
/* if you change this parameter, 
   you MUST change the way of calculation of FPP scatt. angle. */

/** for correction ****************************************************/
struct runparm {
  double m1;                /* Projectile Mass in MeV */
  double m2;                /* Target Mass in MeV */
  double m3;                /* Ejectile Mass in MeV */
  double m4;                /* Residual Target Mass in MeV */
  double K1;                /* Beam Energy */
  double grang;             /* GR Angle */
  double bgr;               /* GR magnetic field in mT */
  double brhogr;            /* GR Brho Tm/c */
  double dpdx;              /* Dispersion of GR */
  double ytgt;              /* Vertical Positon at the Target */
  double dex[2];            /* Fine tuning parameter for Ex */
                            /* dex[0] .. offset, 
                               dex[1] .. first order in Ex */
  double z3;                /* Charge state of the particle 3 */
  double rho;               /* Orbit radius for paritlces with delta=0 */
  char grmode[10];          /* GR MODE NORM or UNF */
  int  igrmode;             /* 0..NORM, 1..UNF */
  double rfc[2];            /* GRRF gate center */
  double rfw;               /* GRRF gate width */
  double yp[2];             /* Y gate peak */
  double yb[4];             /* Y gate back [0]:1L [1]:1H [2]:2L [3]:2H */
  double grpid1[2];         /* PID gate by GR SC1 */
};

/* startup.c */
int startup(int argc,char *argv[],int *fd,char *hbfile);
int writepar(FILE *fd,struct runparm *parm);
