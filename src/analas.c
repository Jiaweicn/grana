/*
Build後のデータを読むプログラム
          Ver. 0.2  データをRegionごとに切り分けられる
                    NIM-IN-BIT の再現に成功
97/07/20  Ver. 0.3  FERA,INPUT_REG をサポート
97/07/21  Ver. 0.31 FERET をサポート
97/07/23  Ver. 0.4  VDC3377 をサポート
97/07/23  Ver. 0.41 event buffer の初期化プロセスを改良(速度25%向上)
97/07/23  Ver. 0.5  MWPC をサポート obsoleted.
97/07/26  Ver. 0.6  MWPC をサポート
97/07/26  Ver. 0.7  Booking のタイミングを変更
97/08/01  Ver. 0.71 いろいろ変更(ray trace の一歩手前まで)
                    -vdc オプションを採用
97/08/06  Ver. 0.8  ray trace をサポート
          Ver. 0.9  Histgram-ID の付けかえ
                    Vertex routine をサポート
97/09/12  Ver. 1.0  FPP 偏極測定をサポート
                    VDC raytracing を改良(最小二乗法を導入)
97/09/26  Ver. 1.1  Histgram をやや変更
                    run2061 に最適化
97/10/12  Ver. 1.2  Y-cut Background を二分割
97/10/15  Ver. 1.3  Event ID の定義を C-style に変更
                    VDC の cluster数が想定値を超えた時に発生するバグをfix
97/12/18  Ver. 1.4  フロー制御・VDC整理法変更のためのコンパイルオプションを設定
                    hbfile name のバグをfix
98/02/28            -global, -online, -port の各オプションを採用
98/04/11            Ctrl-c によるプログラムの中止処理をサポート
98/04/20            -b オプションを採用
98/05/01  Ver. 1.5  ２月以来の開発項目をFPPに対しても有効に
                    FPP解析のためのコンパイルオプションを設定
                    analib5.f を analib6.f, histdef.f に分割
                    analyzing_v1.4.c を analyzing_v1.5.c, correction.c に分割
98/05/12            MWPC のルーチンを改良
98/07/04  Ver. 1.51 FERA, FERET の word count をチェックするルーチンを追加
                    3351PADC のルーチンを追加
98/07/30            縦偏極用コンパイルオプション -D_VER_POL を設定
99/03/24            Region Header の扱いを一部変更
                    Unknown region は case by case に対応する
		    Region f(Check Sum) をサポート
99/06/13  Ver. 1.52 VDC Least Kai-square ルーチンを変更
                    VDC time-pos table の format を変更
99/11/02  Ver. 1.53 VDC Reflect correction を廃止
00/01/06  Ver. 1.54 _VDC_TWOWIRE_ON 使用時に vdc->dis が ＋方向に tail を
                    引くバグをfix
                    BLP をサポート
00/01/13  Ver. 1.55 Event header 類のエラーメッセージを一部変更
                    BLP のエラーチェックを甘くするコンパイルオプション
                      make blp2 を追加
00/01/13  Ver. 1.56 anablp.c を v1.1 に。
                    Camac Scaler の最大値を規定する SC_MAX を定義
00/01/21  analas Ver.0.1
                    ana_v1.56 から分離。LAS を含んだルーチンとする。
                    将来の(p,2p)対応の可能性を残してGR関連ルーチンは
                    そのまま残す。
00/01/28  analas Ver.1.0
                    とりあえず実用段階に入る
                    GR を無視すrためのコンパイルオプション -D_GR_OFF を定義。
                    LAS の Matrix 計算のためのルーチン lasmat を定義。
00/01/31            コンパイルオプション -D_LINUX を定義
                    (linux というよりは gcc か？)
                    -D_LINUX で hstini(end)_g(f)_ が
                                hstini(end)_g(f)__ となる。
                    timedata のデータ型を time_t に変更
                    histid のデータ型を int に変更
00/04/02            main関数の型を void から int に変更
00/07/03            pos_scinti, particleid の型が short になっていた
                    Bug を fix
00/12/07            Change FHSIZE to FIHSIZE to compile on AIX 
                    Add _AIX to avoid bite swap on AIX         
05/07/18   Ver.1.1  ana との整合性のために grcr の定義を追加
                    Global section の名前を引数から定義可能にした。
                    hbfile, gname の fortran関数への渡し方を変更
05/10/05            Add compile option _LAS_VDC_XU
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <signal.h>


#include "ana9.h"
#include "config.h"
#include "vectcalc.h"
#include "router.h"

/*double pawc_[2000000];*/
int fd;
int readbyte,rp,wp;
int eventrp;
char comname[40];
long lgname,lhbname;
char gname[L_3377];

int flag; /* option flag */

int blocksize,numofevent,fieldsize,fsizecount;
int eventsize,numoffield,eventcount,fieldcount,bsizecount,esizecount;
unsigned short regionid,regionsize;

size_t SIZEOFBLK,WORDOFBLK;
unsigned short *buff,*rawbuff;
unsigned long  block,blockskip,event,eventerror,regionerror;
unsigned long  totalbyte;

/* virtual projection plane */
double grplane1[]=GR_PL1;
double grplane2[]=GR_PL2;
double grplane3[]=GR_PL3;
double grplane4[]=GR_PL4;
double grplane5[]=GR_PL5;
double grplane6[]=GR_PL6;
double grplane7[]=GR_PL7;
double grplane8[]=GR_PL8;
double grplane9[]=GR_PL9;
double grplane10[]=GR_PL10;
double grplane11[]=GR_PL11;

double lasplane1[]=LAS_PL1;
double lasplane2[]=LAS_PL2;
double lasplane3[]=LAS_PL3;
double lasplane4[]=LAS_PL4;
double lasplane5[]=LAS_PL5;
double lasplane6[]=LAS_PL6;
double lasplane7[]=LAS_PL7;
double lasplane8[]=LAS_PL8;
double lasplane9[]=LAS_PL9;
double lasplane10[]=LAS_PL10;
double lasplane11[]=LAS_PL11;

double *grplane[NPL];
double *lasplane[NPL];


/************* Event buffer ***********/
short eventid[16];         /* for INPUT register */
short fcet_id[16];         /* for FCET-INPUT register */
unsigned short adc[16];     /* for BLP */
unsigned short tdc[16];     /* for BLP */

unsigned int scaler[N_SCA][16];          /* for CAMAC scaler */
int scalerno;

short fera_gr[40];          /* for GR-FERA */
short feret_gr[40];         /* for GR-FERET */

short fera_las[32];          /* for LAS-FERA */
short feret_las[32];         /* for LAS-FERET */


/** for 3377 VDC ******************************************************/
/** for GR ************************************************************/
struct chamber vdc_fx=VDCPOS_FX;  /* chamber configuration */
struct chamber vdc_fu=VDCPOS_FU;
struct chamber vdc_fv=VDCPOS_FV;
struct chamber vdc_rx=VDCPOS_RX;
struct chamber vdc_ru=VDCPOS_RU;
struct chamber vdc_rv=VDCPOS_RV;

struct vdcdata grfx,grfu,grrx,grru;

unsigned int eff_vdc; /* for efficiency check */

double rayvec_vdc_x[3]; /* ray vector calculated by VDC-data  */
double rayvec_vdc_u[3];
double rayvec_vdc[3];

double plane_vdc_x[4]; /* plane containing ray calculated by VDC-data*/
double plane_vdc_u[4];

double vdcpos[NPL][3];        /*  position decided by vdc */
double vdcpos10_c[3];         /* corrected positon on focal plane */

double theta_x_ray;             
/* Angle between center ray at vdcpos[10] and central ray,
  which is used for correction of spin rotation in GR.*/
int good_vdc;                /*  indicate good ray tracing */
short vdc_rayid[RAYE];       /* error in ray tracing */
short vdc_chamcheck;  /* check for if chambers detect particle */
double ex_energy;            /* excitation energy */
double theta_x[3],theta_y[3]; /* scattering anble */
/* 0:focal plane 1:focal plane corrected 2:carbon block */

/** for LAS ***********************************************************/
struct chamber las_fx=LASVDC_FX;  /* chamber configuration */
struct chamber las_fu=LASVDC_FU;
struct chamber las_fv=LASVDC_FV;
struct chamber las_rx=LASVDC_RX;
struct chamber las_ru=LASVDC_RU;
struct chamber las_rv=LASVDC_RV;

struct vdcdata lasfx,lasfu,lasrx,lasru,lasfv,lasrv;
unsigned int eff_las; /* for efficiency check */

double rayvec_las_u[3]; /* ray vector calculated by VDC-data  */
double rayvec_las_x[3];
double rayvec_las_v[3];


double rayvec_las[3];
double plane_las_u[4]; /* plane containing ray calculated by VDC-data*/
double plane_las_x[4];
double plane_las_v[4];

double laspos[NPL][3];        /*  position decided by vdc */
double laspos10_c[3];         /* corrected positon on focal plane */
double laspsca[3];         /* corrected positon for scattering from H. */

int good_las;                /*  indicate good ray tracing */
short las_rayid[RAYE];       /* error in ray tracing */
short las_chamcheck;  /* check for if chambers detect particle */
double ex_las;            /* excitation energy */
double theta_x_las[2],theta_y_las[2]; /* scattering anble */
/* 0:focal plane 1:focal plane corrected */

/** for various cuts in GR ********************************/
int active_slit;
    /* indicate veto by active slits
          0:No hit on active slit
	  1:Hit upper slit
	  2:Hit lower slit
	  4:Hit outer slit
	  8:Hit inner slit    */
int theta_x_cut; 
    /* indicate result of theta_x[1] cut 
          0:initial value
          1:within good angle
         -1:lower of peak
         -2:upper of peak  */
int theta_y_cut; 
    /* indicate result of theta_y[1] cut 
          0:initial value
          1:within good angle
         -1:lower of peak
         -2:upper of peak  */
int y_cut;
    /* indicate result of vdcpos10_c[1] cut 
          0:initial value
          1:peak
         -1:lower of peak
         -2:upper of peak  */
int y_back;
    /* indicate back groud of y-cut
          0:initial value
          1:back ground    */

/** for various cuts in LAS ********************************/
int rf_cut_las;

/** for MWPC_PCOS **********************************************************/
struct chamber pcpos_x1=PCPOS_X1; /* chamber configuration */
struct chamber pcpos_x2=PCPOS_X2;
struct chamber pcpos_u3=PCPOS_U3;
struct chamber pcpos_v3=PCPOS_V3;
struct chamber pcpos_u4=PCPOS_U4;
struct chamber pcpos_v4=PCPOS_V4;

struct mwpcdata pc_x1,pc_x2,pc_u3,pc_v3,pc_u4,pc_v4;
struct mwpcdata *gr_mwpc[6]; /* pointer for GR MWPC data */
         /* gr_mwpc[0]   pc_x1
            gr_mwpc[1]   pc_x2
            gr_mwpc[2]   pc_u3
            gr_mwpc[3]   pc_v3
            gr_mwpc[4]   pc_u4
            gr_mwpc[5]   pc_v4 */

short good_pc;           /* indicate good event in raytracing by PC */
unsigned int eff_pc,eff_x;     /* for efficiency check in raytracing by PC */
short pc_chamcheck;  /* check for if chambers detect particle */
short pc_rayid[RAYE];       /* error in ray tracing */
double rayvec_pc_u[3]; /* ray vector calculated by VDC-data  */
double rayvec_pc_v[3];
double rayvec_pc[3];

double plane_pc_u[4]; /* plane containing ray calculated by VDC-data*/
double plane_pc_v[4];

double pcpos[NPL][3];        /*  position decided by vdc */

/*** for calculating vertex point ********************************/
double vertexpos[4][3];
    /* [0][] nearest point on vdc-ray
       [1][] nearest point on mwpc-ray
       [2][] middle point between [0][] and [1][] 
       [3][] difference between [0][] and [1]    */
double vertexdis;
double vertexcalc[2][3]; /* for calculating vertex point */
double vertextmp[2]; 
    /* vertexpos[0][]=vdcpos[9]+vertextmp[0]*vect_vdc
       vertexpos[1][]=pcpos[9]+vertextmp[1]*vect_pc */

int good_vertex; /* indicate proper vertex position */
int good_angle;  /* indicate that FPP scatt. angle is within the scope */
int good_fpp; /* indicate good FPP event */

double fpp_longvec[3]; /* longitudenal vector of scattering plane */
double fpp_sidevec[3];   /* sideway vector in FPP */
double fpp_normalvec[3]=FPP_N; /* FPP normal vector */ 
double fpp_angle_theta;  /* FPP scattering angle theta */
double fpp_angle_phi;    /* FPP scattering angle phi */
double fpp_theta_u;      /* FPP scattering angle used 2ndlevel trigger */
double fpp_theta_v;
double fpp_dis;          /* distance between X1 and X2 */

/**** Others (GR) *************************************************/
double pos_scinti;  /* for Trig. Scinti. */
double particleid;
short good_pid1;
double pos_scinti2;  /* for Trig. Scinti. 2 */
double particleid2;
short good_pid2;
short good_pid;
double rf_c,rf_cc;     /* corrected RF-time */
struct runparm grcr;


/**** Others (LAS) ************************************************/
short pos_scinti_las[6];  /* for Trig. Scinti. */
short particleid_las[6];
short good_pid_las;  /* bit 0-5 for each scinti, 
                        bit 6 for front three scitti's
                        bit 7 for rear three scitti's */

struct ssddata grssd;  /* for SSD-Ball */

int pcoserror,pc_noch,pc_badcham,pc_hberr;/*tmp*/

void book1d(int histid,double data)
{
  float fdata;
  fdata=data;
  book1f_(&histid,&fdata);
}

void book1(int histid,float data)
{
  book1f_(&histid,&data);
}

void book2d(int histid,double datax,double datay)
{
  float fdatax,fdatay;
  fdatax=datax;fdatay=datay;
  book2f_(&histid,&fdatax,&fdatay);
}

void file_end();
void lascor(double *fppos,double *colpos,double *theta,double *phi);

int main(int argc,char *argv[])
{
  int i;
  struct sigaction act;       /* definition of interrupt routine */
  char hbfile[100]; /* name for hbfile */

  clock();

  pcoserror=0; /* tmp */
  pc_noch=0;
  pc_badcham=0;

  strcpy(comname,argv[0]);
  flag=startup(argc,argv,&fd,hbfile);

  act.sa_handler=file_end;
  sigaction(SIGINT,&act,NULL);

/*** Initialize for GR_VDC *********************/
  if(flag & 0x1) vdc_calb(fd);
  
  for(i=0;i<L_3377;i++){   /* initialize time table */
    grfx.table[i]=-1;grfu.table[i]=-1;
    grrx.table[i]=-1;grru.table[i]=-1;
  }

#ifndef _GR_OFF
  tableread(FXTBL,&grfx);
  tableread(FUTBL,&grfu);
  tableread(RXTBL,&grrx);
  tableread(RUTBL,&grru);
#endif /* _GR_OFF */

/* comment out 00/01/21
  grfx.wire=VDCEVE_L; 
  grfu.wire=VDCEVE_L;
  grrx.wire=VDCEVE_L;
  grru.wire=VDCEVE_L;

  grfx.nclu=VDCNCL;
  grfu.nclu=VDCNCL;
  grrx.nclu=VDCNCL;
  grru.nclu=VDCNCL;*/

  strcpy(grfx.label,"GR_FR_X");
  strcpy(grfu.label,"GR_FR_U");
  strcpy(grrx.label,"GR_RE_X");
  strcpy(grru.label,"GR_RE_U");

  grfx.eff=0;grfu.eff=0;grrx.eff=0;grru.eff=0;eff_vdc=0;

  grfx.tilt_c=cos(vdc_fx.tilt*D_TO_R);
  grfx.tilt_s=sin(vdc_fx.tilt*D_TO_R);
  grfu.tilt_c=cos(vdc_fu.tilt*D_TO_R);
  grfu.tilt_s=sin(vdc_fu.tilt*D_TO_R);
  grrx.tilt_c=cos(vdc_rx.tilt*D_TO_R);
  grrx.tilt_s=sin(vdc_rx.tilt*D_TO_R);
  grru.tilt_c=cos(vdc_ru.tilt*D_TO_R);
  grru.tilt_s=sin(vdc_ru.tilt*D_TO_R);
  rotate_y(grfx.tilt_s,grfx.tilt_c,vdc_fx.wiredirec,vdc_fx.wiredirec);
  rotate_y(grfu.tilt_s,grfu.tilt_c,vdc_fu.wiredirec,vdc_fu.wiredirec);
  rotate_y(grrx.tilt_s,grrx.tilt_c,vdc_rx.wiredirec,vdc_rx.wiredirec);
  rotate_y(grru.tilt_s,grru.tilt_c,vdc_ru.wiredirec,vdc_ru.wiredirec);

/*** Initialize for LAS_VDC *********************/
  for(i=0;i<L_3377;i++){   /* initialize time table */
    lasfx.table[i]=-1;lasrx.table[i]=-1;
    lasfv.table[i]=-1;lasrv.table[i]=-1;
    lasfu.table[i]=-1;lasru.table[i]=-1;
  }


  tableread(LASFXTBL,&lasfx);
  tableread(LASRXTBL,&lasrx);
  strcpy(lasfx.label,"LAS_FR_X");
  strcpy(lasrx.label,"LAS_RE_X");
  lasfx.eff=0;lasrx.eff=0;
  lasfx.tilt_c=cos(las_fx.tilt*D_TO_R);
  lasfx.tilt_s=sin(las_fx.tilt*D_TO_R);
  lasrx.tilt_c=cos(las_rx.tilt*D_TO_R);
  lasrx.tilt_s=sin(las_rx.tilt*D_TO_R);
  rotate_y(lasfx.tilt_s,lasfx.tilt_c,las_fx.wiredirec,las_fx.wiredirec);
  rotate_y(lasrx.tilt_s,lasrx.tilt_c,las_rx.wiredirec,las_rx.wiredirec);

  tableread(LASFVTBL,&lasfv);
  tableread(LASRVTBL,&lasrv);
  strcpy(lasfv.label,"LAS_FR_V");
  strcpy(lasrv.label,"LAS_RE_V");
  lasfv.eff=0;lasrv.eff=0;
  lasfv.tilt_c=cos(las_fv.tilt*D_TO_R);
  lasfv.tilt_s=sin(las_fv.tilt*D_TO_R);
  lasrv.tilt_c=cos(las_rv.tilt*D_TO_R);
  lasrv.tilt_s=sin(las_rv.tilt*D_TO_R);
  rotate_y(lasfv.tilt_s,lasfv.tilt_c,las_fv.wiredirec,las_fv.wiredirec);
  rotate_y(lasrv.tilt_s,lasrv.tilt_c,las_rv.wiredirec,las_rv.wiredirec);

  tableread(LASFUTBL,&lasfu);
  tableread(LASRUTBL,&lasru);
  strcpy(lasfu.label,"LAS_FR_U");
  strcpy(lasru.label,"LAS_RE_U");
  lasfu.eff=0;lasru.eff=0;
  lasfu.tilt_c=cos(las_fu.tilt*D_TO_R);
  lasfu.tilt_s=sin(las_fu.tilt*D_TO_R);
  lasru.tilt_c=cos(las_ru.tilt*D_TO_R);
  lasru.tilt_s=sin(las_ru.tilt*D_TO_R);
  rotate_y(lasfu.tilt_s,lasfu.tilt_c,las_fu.wiredirec,las_fu.wiredirec);
  rotate_y(lasru.tilt_s,lasru.tilt_c,las_ru.wiredirec,las_ru.wiredirec);

  eff_las=0;



/*** Initialize for MWPC *********************/
  gr_mwpc[0]=&pc_x1;
  gr_mwpc[1]=&pc_x2;
  gr_mwpc[2]=&pc_u3;
  gr_mwpc[3]=&pc_v3;
  gr_mwpc[4]=&pc_u4;
  gr_mwpc[5]=&pc_v4;

  strcpy(pc_x1.label,"PC_X1");
  strcpy(pc_x2.label,"PC_X2");
  strcpy(pc_u3.label,"PC_U3");
  strcpy(pc_v3.label,"PC_V3");
  strcpy(pc_u4.label,"PC_U4");
  strcpy(pc_v4.label,"PC_V4");

  pc_x1.tilt_c=cos(pcpos_x1.tilt*D_TO_R);
  pc_x1.tilt_s=sin(pcpos_x1.tilt*D_TO_R);
  pc_x2.tilt_c=cos(pcpos_x2.tilt*D_TO_R);
  pc_x2.tilt_s=sin(pcpos_x2.tilt*D_TO_R);
  pc_u3.tilt_c=cos(pcpos_u3.tilt*D_TO_R);
  pc_u3.tilt_s=sin(pcpos_u3.tilt*D_TO_R);
  pc_v3.tilt_c=cos(pcpos_v3.tilt*D_TO_R);
  pc_v3.tilt_s=sin(pcpos_v3.tilt*D_TO_R);
  pc_u4.tilt_c=cos(pcpos_u4.tilt*D_TO_R);
  pc_u4.tilt_s=sin(pcpos_u4.tilt*D_TO_R);
  pc_v4.tilt_c=cos(pcpos_v4.tilt*D_TO_R);
  pc_v4.tilt_s=sin(pcpos_v4.tilt*D_TO_R);
  rotate_y(pc_x1.tilt_s,pc_x1.tilt_c,pcpos_x1.wiredirec,pcpos_x1.wiredirec);
  rotate_y(pc_x2.tilt_s,pc_x2.tilt_c,pcpos_x2.wiredirec,pcpos_x2.wiredirec);
  rotate_y(pc_u3.tilt_s,pc_u3.tilt_c,pcpos_u3.wiredirec,pcpos_u3.wiredirec);
  rotate_y(pc_v3.tilt_s,pc_v3.tilt_c,pcpos_v3.wiredirec,pcpos_v3.wiredirec);
  rotate_y(pc_u4.tilt_s,pc_u4.tilt_c,pcpos_u4.wiredirec,pcpos_u4.wiredirec);
  rotate_y(pc_v4.tilt_s,pc_v4.tilt_c,pcpos_v4.wiredirec,pcpos_v4.wiredirec);


  fpp_dis=(pcpos_x2.position[2]-pcpos_x1.position[2])*sqrt(2.); 
                /* for 2nd level trigger check */

  pc_x1.eff=0;pc_x2.eff=0;pc_u3.eff=0;pc_v3.eff=0;
  pc_u4.eff=0;pc_v4.eff=0;eff_x=0;eff_pc=0;

/*** Initialize for CAMAC scaler ************/
  for(i=0;i<16;i++){
    scaler[0][i]=0;scaler[1][i]=0;
  }

/* Initialize virtual projection plane */
  grplane[0]=grplane1;
  grplane[1]=grplane2;
  grplane[2]=grplane3;
  grplane[3]=grplane4;
  grplane[4]=grplane5;
  grplane[5]=grplane6;
  grplane[6]=grplane7;
  grplane[7]=grplane8;
  grplane[8]=grplane9;
  grplane[9]=grplane10;
  grplane[10]=grplane11;
  makeplane(grplane);   /* make equation of virtual projection plane */

  lasplane[0]=lasplane1;
  lasplane[1]=lasplane2;
  lasplane[2]=lasplane3;
  lasplane[3]=lasplane4;
  lasplane[4]=lasplane5;
  lasplane[5]=lasplane6;
  lasplane[6]=lasplane7;
  lasplane[7]=lasplane8;
  lasplane[8]=lasplane9;
  lasplane[9]=lasplane10;
  lasplane[10]=lasplane11;
  makeplane(lasplane);   /* make equation of virtual projection plane */

  lgname=strlen(gname);
  lhbname=strlen(hbfile);

#ifdef _LINUX
  if(flag & 0x8) hstini_g__(gname,lgname);
  else {
    hstini_f__(hbfile,lhbname);
  }
#else
  if(flag & 0x8) hstini_g_(gname,lgname);
  else {
    hstini_f_(hbfile,lhbname);
  }
#endif
  block=0;blockskip=0;event=0;eventerror=0;regionerror=0;
  totalbyte=0;

  hstdef_();

 newblock:
  while(1){
    if(flag & 0x10){ /* if online option is given */
      int getbuff;
      getbuff=tcp_get_buf((char *)rawbuff);
      switch(getbuff){
      case -1:
	printf("Fail to get rawdata from router.\n");
	exit(-1);
	break;
      case 1:
	readbyte=0;
	break;
      case 0:
	readbyte=SIZEOFBLK;
	break;
      }
    }
    else{ /* if it is offline analysis */
      readbyte=read(fd,rawbuff,SIZEOFBLK);
/*    printf("readbytes = %d \n",readbyte);*/
      if(readbyte==-1) {
	printf("Error(read file)!\n");
	close(fd);
	exit(-1);
      }
  
/* if you need flow control, 
   you must give compile option -D_KEEP_BLOCK_SIZE. */
#ifdef _KEEP_BLOCK_SIZE 
      while(readbyte!=SIZEOFBLK && readbyte !=0){
	ssize_t tmpreadbyte;
	tmpreadbyte=
	  read(fd,rawbuff+(readbyte/sizeof(short)),SIZEOFBLK-readbyte);
	printf("readbyte:%d  tmpreadbyte:%ld\n",readbyte,tmpreadbyte);
	readbyte+=tmpreadbyte;
      }
#endif
    }

    if(readbyte==0) {
      printf("File end!\n");
      file_end();
    }

    totalbyte+=readbyte;

#ifndef _AIX
    swab(rawbuff,buff,readbyte);
#else
	buff=rawbuff;
#endif	
    wp=0;

/******************************************************************
  Block Header Detection
 ******************************************************************/

    if(buff[wp]==0xffff){
      int i;
      time_t timedata;
      
      bsizecount=0;eventcount=0;
      scalerno=0;
      if(buff[++wp]!=BHSIZE) 
	printf("Inconsistent Block Header Size! : %d word\n",buff[wp]);
      switch(buff[++wp]){
      case 0x0f01: /* if Run Start Block*/
	printf("#################################\n");
	printf("Run Start Block.(%ld)\n",block);
	blocksize=buff[++wp];wp+=2;

	if(buff[wp+1]==0) ++wp; /* Event flag*/
	++wp; /* Data Format Version */

#ifndef _AIX
        swab(&rawbuff[++wp],&timedata,sizeof(time_t));
#else
        timedata=rawbuff[++wp];
#endif

/*	printf("byteorder %x\n",timedata); */	
	++wp;

	timedata=ntohl(*(int *)&rawbuff[++wp]);
	printf("%s",ctime(&timedata));
	++wp;/* time stamp */

	printf("run%d\t",buff[++wp]); /* run # */
	printf("%s\n",(char *)&rawbuff[++wp]); /* comment */
	printf("#################################\n");
	block++;

	goto newblock;

      case 0x0f02: /* if Run End Block */
	printf("Run End Block.(%ld)\n",block);
	blocksize=buff[++wp];
	wp++; /* Block # */
	wp++;        /* event # */

	if(buff[wp+1]==0) ++wp; /* Event flag*/
	++wp; /* Data Format Version */

#ifndef _AIX
	swab(&rawbuff[++wp],&timedata,sizeof(int));
#else
	timedata=rawbuff[++wp];
#endif	

/*	printf("byteorder %x\n",timedata); */
	++wp;

	timedata=ntohl(*(int *)&rawbuff[++wp]);
/*	printf("%s",ctime(&timedata));*/
	++wp;/* time stamp */

	printf("run %d\t",buff[++wp]); /* run # */
	printf("%s\n",(char *)&rawbuff[++wp]); /* comment */
	file_end();
/*	goto newblock;*/

      case 0x0000: /*if Data Block*/
	if((block & 0xff)==0xff){
	  printf("Data Block : %ld th\n",block);
	  if(!block) printf("Analyzing ratio: %5.2f\n"
			    ,100.*(block-blockskip)/block);
	}
	blocksize=buff[++wp];	break;
      default:
	printf("b%ld:Inconsistent Block ID : %x\n",block,buff[wp]);
	blocksize=buff[++wp];
	exit(-1);
	break;
      }

      if(buff[++wp]!=block) {
	printf("Inconsistent Block Number! read:%d th  count:%ld th\n",
	       buff[wp],block+1);
	 /* Block is incremented to keep "block counting rule". */

	while(buff[wp]!=block) {
/*	  printf("I found a skipped block!\n");*/
	  blockskip++;block++;
	  if(buff[wp]<block) {
	    printf("Fetal error!\t buff[wp] < block \n");
#ifdef _BLP_ON
	    printf("Change block count. from %d(counted) to %d(read).\n",
		   block,buff[wp]);
	    block=buff[wp];
#else	    
	    exit(-1);
#endif
	  }
	}
      }
      numofevent=buff[++wp];
      block++;
      

/**********************************************************************
 Event Header Detection
 **********************************************************************/
      while(blocksize>bsizecount+2 && numofevent>eventcount){

int i;

for(i=0;i<16;i++) {
  eventid[i]=0;
  fcet_id[i]=-1;
  adc[i]=0;
  tdc[i]=0;
}

for(i=0;i<32;i++) {
  fera_gr[i]=-1;
  feret_gr[i]=-1;
  fera_las[i]=-1;
  feret_las[i]=-1;
}


/** Initialize for GR_VDC *******************************************/
grfx.wire=0;grfu.wire=0;
grrx.wire=0;grru.wire=0;

grfx.nclu=0;grfx.nlclu=0;
grfu.nclu=0;grfu.nlclu=0;
grrx.nclu=0;grrx.nlclu=0;
grru.nclu=0;grru.nlclu=0;

grfx.kai=-1;
grfu.kai=-1;
grrx.kai=-1;
grru.kai=-1;

for(i=0;i<POSE;i++) {
  grfx.posid[i]=0;grfu.posid[i]=0;grrx.posid[i]=0;grru.posid[i]=0;}

grfx.good=0;grfu.good=0;grrx.good=0;grru.good=0;good_vdc=0;

for(i=0;i<RAYE;i++){
  vdc_rayid[i]=0;}
vdc_chamcheck=0;

/** Initialize for various cuts (GR)****/
active_slit=0;theta_x_cut=0;theta_y_cut=0;y_cut=0;y_back=0;
good_pid=0;good_pid1=0;good_pid2=0;

/** Initialize for various cuts (LAS) ****/
good_pid_las=0;
rf_cut_las=0;

/** Initialize for LAS_VDC *******************************************/
lasfx.wire=0;lasrx.wire=0;
lasfx.nclu=0;lasfx.nlclu=0;
lasrx.nclu=0;lasrx.nlclu=0;
lasfx.kai=-1;lasrx.kai=-1;
lasfx.good=0;lasrx.good=0;

lasfv.wire=0;lasrv.wire=0;
lasfv.nclu=0;lasfv.nlclu=0;
lasrv.nclu=0;lasrv.nlclu=0;
lasfv.kai=-1;lasrv.kai=-1;
lasfv.good=0;lasrv.good=0;

lasfu.wire=0;lasru.wire=0;
lasfu.nclu=0;lasfu.nlclu=0;
lasru.nclu=0;lasru.nlclu=0;
lasfu.kai=-1;lasru.kai=-1;
lasfu.good=0;lasru.good=0;

for(i=0;i<POSE;i++) {
  lasfx.posid[i]=0;lasrx.posid[i]=0;
  lasfv.posid[i]=0;lasrv.posid[i]=0;
  lasfu.posid[i]=0;lasru.posid[i]=0;
}
good_las=0;

for(i=0;i<RAYE;i++){
  las_rayid[i]=0;}
las_chamcheck=0;

/***** Initialize for MWPC ******/
for(i=0;i<POSE;i++) {
  pc_x1.posid[i]=0;pc_x2.posid[i]=0;pc_u3.posid[i]=0;
  pc_v3.posid[i]=0;pc_u4.posid[i]=0;pc_v4.posid[i]=0;}

pc_x1.good=0;pc_x2.good=0;pc_u3.good=0;
pc_v3.good=0;pc_u4.good=0;pc_v4.good=0;good_pc=0;

pc_x1.wire=0;pc_x2.wire=0;pc_u3.wire=0;
pc_v3.wire=0;pc_u4.wire=0;pc_v4.wire=0;

pc_chamcheck=0;
for(i=0;i<RAYE;i++){
  pc_rayid[i]=0;
}

/***** Initialize for FPP ******/
good_vertex=0;good_angle=0;good_fpp=0;

/***** Initialize for SSD-Ball ******/
grssd.k_adc=0;grssd.k_tdc=0;

/**********************************************/

	if(buff[++wp]==0xffdf) {
	  eventrp=wp;
	  esizecount=0;fieldcount=0;
	  event++;eventcount++;
	  if(buff[++wp]!=EHSIZE) 
	    printf("b %ld:e %d:Inconsistent Event Header Size! : %d words\n",
		   block,eventcount,buff[wp]);
	  bsizecount+=buff[wp];
	  switch(buff[++wp]){
	  case 0x0001:
/*	    printf("Block End Event. : %d th\n",event);*/
	    break;
	  case 0x0000:
/*	    printf("Data Event : %d th\n",event);*/
	    break;
	  default:
	    printf("b %ld:e %d:Inconsistent Event ID : %x\n",
		   block,eventcount,buff[wp]);
	  }

	  eventsize=buff[++wp];
	  bsizecount+=eventsize;
	  if(buff[++wp]!=eventcount-1)
	    printf("b %ld:e %dInconsistent Event Number! read:%dth count:%dth\n"
		   ,block,eventcount,buff[wp]+1,eventcount);
	  if(buff[++wp]!=N_FIELD){
	    printf("Inconsistent Number of Fields! %d\n",buff[wp]);
	    exit(-1);}
	  numoffield=buff[wp];

/**********************************************************************
 Field Header Detection
 **********************************************************************/
	  while(eventsize>esizecount && numoffield > fieldcount){
	    int k;
	    short base_ch,channel;	
	    unsigned short data;
	    int j,histid1,histid2; 
	    
	    if(buff[++wp]==0xffcf) {
	      fsizecount=0;
	      if(buff[++wp]!=FIHSIZE) 
		printf("Inconsistent Field Header Size! :%d words\n",buff[wp]);
	      esizecount+=buff[wp];fieldcount++;
	      if(buff[++wp]!=0x0000) 
		printf("Inconsistent Field ID! :%x\n",buff[wp]);
	      fieldsize=buff[++wp];
	      esizecount+=fieldsize;
	      
/*****************************
  Event ID routine 
 *****************************/
	      event_id();
/*********************************************************************/
/*********************************************************************/
/* region function is here*/
/*********************************************************************/
/*********************************************************************/

	      while(fieldsize>fsizecount){		
		data=buff[++wp];
		regionid=(data>>12);
		regionsize=(data & 0x0fff);
		fsizecount+=(regionsize+1);

		switch(regionid) { 
/**INPUT_REG***********************************************************/
	        case INPUT_REG: /* FCET input register */
		  input_reg();
		  break;
/**ADC****************************************************************/
		case ADC:
		  adcread();
		  break;
/**TDC****************************************************************/
		case TDC:
		  tdcread();
		  break;
/**FERA***************************************************************/
		case FERA:
		  fera();
		  break;
/**FERET*******************************************************************/
		case FERET:
		  feret();
		  break;
/**3377VDC******************************************************************/
		case VDC_3377:
		  vdc_3377();
		  break;		    
/**MWPC_PCOS***************************************************************/
		case MWPC_PCOS:
		  mwpc_pcos();
		  break;
/**SCALER*******************************************************************/
		case SCALER:
		  camac_scaler();
		  break;
/**3351SSD******************************************************************/
		case SSD_3351:
		  ssd_3351();
		  break;
/**CHECKSUM******************************************************************/
		case CHECK_SUM:
		  wp+=regionsize;
		  break;
/**************************************************************************/
		default:
		  printf("%s:b %ld:e %d:Unknown region ID.(%x,%d)\n",
			 comname,block,eventcount,regionid,regionsize);
		  wp+=regionsize;
		  break;
		}
	      }
/**Analyzing****************************************************************/
#include "analyzing_las.c"
/**Analyzing****************************************************************/

/**Booking******************************************************************/
#include "booking_las.c"
/* Booking******************************************************************/
/*********************************************************************/
	    }
	    else {
	      printf("I cannot find fieldheader in the head of field!\n");
	      exit(-1);
	    }
	    if(fieldsize!=fsizecount){
	      printf("%s:b %ld:e %d:Inconsistent Field Size! read:%d count:%d\n"
		     ,comname,block,eventcount,fieldsize,fsizecount);
	      for(i=eventrp;i<eventrp+eventsize+EHSIZE+1;i++)
		printf("%4x ",buff[i]);
	      printf("\n");
	    }
	  }
	    
	  if(fieldcount!=numoffield){
	    printf("Inconsistent Number of Fields! read:%d count:%d\n",
		   numoffield,fieldcount);
	    exit(-1);
	  }
	  if(eventsize!=esizecount) {
	    printf("b %ld:e %d:Inconsistent Event Size! read:%d count:%d\n",
		   block,eventcount,eventsize,esizecount);
	    exit(-1);
	  }
	}
	else {
	  printf("b %ld:e %d:There is no event header at the head of event!\n",
			 block,eventcount);
	  eventerror++;bsizecount++;

/* tmp */
  for(i=wp-eventsize-EHSIZE;i<wp;i++)
    printf("%4x ",buff[i]);
  printf("\n");
    printf("#### %4x ####\n",buff[wp]);
  for(i=wp+1;i<wp+64;i++)
    printf("%4x ",buff[i]);
  printf("\n");
/* tmp */
	  
	  exit(-1);
	}
}
/**********************************************************************
 Block trailer Detection
 **********************************************************************/
      if(buff[++wp]==0xffef){
	int i;
	bsizecount+=buff[++wp];
	if(bsizecount!=blocksize){
	  printf("b %ld:e %d:Inconsistent Block Size! read:%d count:%d\n",
		 block,eventcount,blocksize,bsizecount);
	  exit(-1);
	}
	if(eventcount!=numofevent) {
	  printf("b %ld:e %d:Inconsistent Number of Events! read:%d count:%d\n",
		 block,eventcount,numofevent,eventcount);
	  exit(-1);
	}
/*	printf("WORDOFBLK-wp = %d\n",WORDOFBLKD-wp);*/
/*	for(i=0;i<WORDOFBLK-wp;i++)
	  printf("%d ",buff[wp+i]);
	printf("\n%h\n",wp+i);*/

      }
      else {
	printf("b %ld:%d:I cannnot find block trailer in the end of bolck!\n",
	       block,buff[wp]);
	exit(-1);
      }
    }
    else {
      printf("b %ld:I cannot find blockheader in the head of block!\n",
	     block);
      exit(-1);
    }
  }
}

void file_end(){
  int i,j;
  if(flag & 0x10) tcp_close();
  else close(fd);
  free((void *)rawbuff);free((void *)buff); /* Free the buffer */
  printf("I have read %ld bytes.\n",totalbyte);
  printf("Block Analyzed:%ld\tSkipped:%ld\n",block-blockskip,blockskip);
  printf("Event Analyzed:%ld\n",event);
#ifdef _LINUX
  if(flag & 0x8) hstend_g__();
  else hstend_f__();
#else
  if(flag & 0x8) hstend_g_();
  else hstend_f_();
#endif
  printf("CPU time = %f sec\n",clock()/1000000.);
  printf("MWPC hb-error:%d badcham:%d noch:%d\n",
	 pc_hberr,pc_badcham,pc_noch);
  
  for(i=0;i<16;i++){
    printf("SCALER %x:",i);
    for(j=0;j<N_SCA;j++) printf("%12u",scaler[j][i]);
    printf("\n");
  }
  
  printf("\nEfficiency\n");
  if(grfx.eff)
    printf("GR_FX :%6.4f%12u\n",((float)eff_vdc)/grfx.eff,grfx.eff);
  if(grfu.eff)
    printf("GR_FU :%6.4f%12u\n",((float)eff_vdc)/grfu.eff,grfu.eff);
  if(grrx.eff)
    printf("GR_RX :%6.4f%12u\n",((float)eff_vdc)/grrx.eff,grrx.eff);
  if(grru.eff)
    printf("GR_RU :%6.4f%12u\n",((float)eff_vdc)/grru.eff,grru.eff);
  for(i=0;i<2;i++){
    if(gr_mwpc[i]->eff)
      printf("%s :%6.4f%12u\n",gr_mwpc[i]->label,
	     ((float)eff_x)/gr_mwpc[i]->eff,gr_mwpc[i]->eff);
  }
  for(i=2;i<6;i++){
    if(gr_mwpc[i]->eff)
      printf("%s :%6.4f%12u\n",gr_mwpc[i]->label,
	     ((float)eff_pc)/gr_mwpc[i]->eff,gr_mwpc[i]->eff);
  }
  if(lasfx.eff)
    printf("LAS_FX :%6.4f%12u\n",((float)eff_las)/lasfx.eff,lasfx.eff);
  if(lasfu.eff)
    printf("LAS_FU :%6.4f%12u\n",((float)eff_las)/lasfu.eff,lasfu.eff);
  if(lasfv.eff)
    printf("LAS_FV :%6.4f%12u\n",((float)eff_las)/lasfv.eff,lasfv.eff);
  if(lasrx.eff)
    printf("LAS_RX :%6.4f%12u\n",((float)eff_las)/lasrx.eff,lasrx.eff);
  if(lasru.eff)
    printf("LAS_RU :%6.4f%12u\n",((float)eff_las)/lasru.eff,lasru.eff);
  if(lasrv.eff)
    printf("LAS_RV :%6.4f%12u\n",((float)eff_las)/lasrv.eff,lasrv.eff);

  if(block-blockskip){
    printf("\nAnalyzing ratio: %5.2f\n",100.*(block-blockskip)/block);
  }
  exit(0);
}

