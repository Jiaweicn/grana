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
00/01/07  anablp Ver.1.0
                    ana_v1.54 から分離。BLP専用ルーチンとして、
                    不要部分を削除。
                    エラー検出を甘くした。
00/01/13         Ver.1.1
                    データ読み込みルーチンを大幅に変更
                    ヘッダー情報を信じる立場から、
                    読み込みバイト数を信じる立場に変更。
00/01/31            コンパイルオプション -D_LINUX を定義
                    (linux というよりは gcc か？)
                    -D_LINUX で hstini(end)_g(f)_ が
                                hstini(end)_g(f)__ となる。
                    timedata のデータ型を time_t に変更
                    histid のデータ型を int に変更

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
#include "router.h"

#define FSIZSC  0x0046
#define FSIZEV  0x0014
#define FSIZDE  0x0026

#define BLP1EV  0x1000
#define BLP2EV  0x2000
#define BLPDEV  0x3000
#define SCALEV  0x8000

#define RSIZSC  0x0010
#define RSIZEV  0x0008

/*double pawc_[2000000];*/
int fd;
int readbyte,rp,wp;
int eventrp;
char comname[40];

int flag; /* option flag */

int blocksize,numofevent,fieldsize,fsizecount;
int eventsize,numoffield,eventcount,fieldcount,bsizecount,esizecount;
unsigned short regionid,regionsize;

size_t SIZEOFBLK,WORDOFBLK;
unsigned short *buff,*rawbuff;
unsigned long  block,blockskip,event,eventerror,regionerror;
unsigned long  totalbyte;


/************* Event buffer ***********/
short eventid[16];         /* for INPUT register */
short fcet_id[16];         /* for FCET-INPUT register */
unsigned short adc[16];     /* for BLP */
unsigned short tdc[16];     /* for BLP */

unsigned int scaler[N_SCA][16];          /* for CAMAC scaler */
int scalerno;

/**** Others ****************************************************/

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
void write_event();

void main(int argc,char *argv[])
{
  int i;
  struct sigaction act;       /* definition of interrupt routine */
  char hbfile[100]; /* name for hbfile */


  clock();

  strcpy(comname,argv[0]);
  flag=startup(argc,argv,&fd,hbfile);
  act.sa_handler=file_end;
  sigaction(SIGINT,&act,NULL);

/*** Initialize for CAMAC scaler ************/
  for(i=0;i<16;i++){
    scaler[0][i]=0;scaler[1][i]=0;
  }


#ifdef _LINUX
  if(flag & 0x8) hstini_g__();
  else {
    hstini_f__(hbfile);
  }
#else
  if(flag & 0x8) hstini_g_();
  else {
    hstini_f_(hbfile);
  }
#endif
  block=0;blockskip=0;event=0;eventerror=0;regionerror=0;
  totalbyte=0;

  hstdef_();

 newblock:
  while(1){
    int i;
    time_t timedata;
    if(flag & 0x10){ /* if online option is given */
      int getbuff;
      getbuff=tcp_get_buf(rawbuff);
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
	printf("readbyte:%d  tmpreadbyte:%d\n",readbyte,tmpreadbyte);
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
	timedata=rawbuff[++wp];
#endif	
      
    wp=0;

/******************************************************************
  Block Header Detection
 ******************************************************************/

    if(buff[wp]!=0xffff){
      int i;
      printf("b %d:I cannot find blockheader in the head of block! (0x%4x)\n",
	     block,buff[wp]);
      for(i=0;i<12;i++){
	printf("%4x ",buff[i]);
      }
      printf("\n");
    }
      
    bsizecount=0;eventcount=0;
    scalerno=0;
    if(buff[++wp]!=BHSIZE) 
      printf("Inconsistent Block Header Size! : %d word\n",buff[wp]);
    switch(buff[++wp]){
    case 0x0f01: /* if Run Start Block*/
      printf("#################################\n");
      printf("Run Start Block.(%d)\n",block);
      blocksize=buff[++wp];wp+=2;
      
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
	printf("%s",ctime(&timedata));
	++wp;/* time stamp */

	printf("run%d\t",buff[++wp]); /* run # */
	printf("%s\n",&rawbuff[++wp]); /* comment */
	printf("#################################\n");
	block++;

	goto newblock;

      case 0x0f02: /* if Run End Block */
	printf("Run End Block.(%d)\n",block);
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
	printf("%s\n",&rawbuff[++wp]); /* comment */
	goto newblock;

      case 0x0000: /*if Data Block*/
	if((block & 0xff)==0xff){
	  printf("Data Block : %d th\n",block);
	  if(!block) printf("Analyzing ratio: %5.2f\n"
			    ,100.*(block-blockskip)/block);
	}
	blocksize=buff[++wp];	break;
      default:
	printf("b%d:Inconsistent Block ID : %x\n",block,buff[wp]);
	blocksize=buff[++wp];
	exit(-1);
	break;
      }

      if(buff[++wp]!=block) {
	printf("Inconsistent Block Number! read:%d th  count:%d th\n",
	       buff[wp],block+1);
	 /* Block is incremented to keep "block counting rule". */

	while(buff[wp]!=block) {
/*	  printf("I found a skipped block!\n");*/
	  blockskip++;block++;
	  if(buff[wp]<block) {
	    printf("Fetal error!\t buff[wp] < block \n");
	    printf("Change block count. from %d(counted) to %d(read).\n",
		   block,buff[wp]);
	    block=buff[wp];
	  }
	}
      }
      numofevent=buff[++wp];
      block++;
      

/**********************************************************************
 Event Header Detection
 **********************************************************************/
    while(blocksize>bsizecount+2 && numofevent>eventcount){
      unsigned short ehid;
      int i;

      for(i=0;i<16;i++) {
	eventid[i]=0;
	fcet_id[i]=-1;
	adc[i]=0;
	tdc[i]=0;
      }

/**********************************************/

      if(buff[++wp]!=0xffdf) {
	printf("b %d:e %d:There is no event header at the head of event!\n",
	       block,eventcount);
	eventerror++;

	/* tmp */
	for(i=wp-eventsize-EHSIZE;i<wp;i++)
	  printf("%4x ",buff[i]);
	printf("\n");
	printf("#### %4x ####\n     ",buff[wp]);
	for(i=wp+1;i<wp+32;i++)
	  printf("%4x ",buff[i]);
	printf("\n\n");
	/* tmp */
      }  

      eventrp=wp;
      esizecount=0;fieldcount=0;
      event++;eventcount++;
      if(buff[++wp]!=EHSIZE) {
	printf("b %d:e %d:Inconsistent Event Header Size! : %x\n",
	       block,eventcount,buff[wp]);
	for(i=eventrp-eventsize-EHSIZE;i<eventrp+buff[wp+2]+EHSIZE+1;i++)
	  printf("%4x ",buff[i]);
	printf("\n\n");
      }

      bsizecount+=EHSIZE;
      ehid=buff[++wp];

      eventsize=buff[++wp];
      bsizecount+=eventsize;
      if(buff[++wp]!=eventcount-1)
	printf("b %d:e %d:Inconsistent Event Number! read:%dth count:%dth\n",
	       block,eventcount,buff[wp]+1,eventcount);
      if(buff[++wp]!=N_FIELD){
	printf("%s:b %d:e %d:Inconsistent Number of Fields! (0x%4x)\n"
	       ,comname,block,eventcount,buff[wp]);
	write_event();
	printf("\n");
      }
      numoffield=N_FIELD;

/**********************************************************************
 Field Header Detection
 **********************************************************************/
      while(eventsize>esizecount && numoffield > fieldcount){
	int k;
	short base_ch,channel;	
	unsigned short data;
	int j,histid1,histid2; 
	int resttdc=1;
	    
	if(buff[++wp]!=0xffcf) {
	  printf("%s:b %d:e %d:",comname,block,eventcount);
	  printf("Wrong Field Header in the head of field! (0x%4x)\n",
		 buff[wp]);
	  write_event();
	  printf("\n");
	}
	
	fsizecount=0;
	if(buff[++wp]!=FIHSIZE) {
	  printf("%s:b %d:e %d:Inconsistent Field Header Size!"
		 ,comname,block,eventcount);
	  printf(" :0x%4x\n",buff[wp]);
	  write_event();
	  printf("\n");
	}
	esizecount+=FIHSIZE;fieldcount++;
	if(buff[++wp]!=0x0000) {
	  printf("%s:b %d:e %d:Inconsistent Field ID! :0x%4x\n"
		 ,comname,block,eventcount,buff[wp]);
	  write_event();
	  printf("\n");
	}
	
	fieldsize=buff[++wp];
	if(buff[wp+1]!=0x2001) { /** check input register **/
	  printf("%s:b %d:e %d:",comname,block,eventcount);
	  printf("Wrong region header.(%x,%4x)\n",
		 buff[wp+1]>>12,buff[wp+1]&0x0fff);
	  write_event();
	  printf("  Change to the INPRT_REG.\n");
	  printf("\n");
	  buff[wp+1]=0x2001;
	}
/*****************************
  Event ID routine 
 *****************************/
	event_id();
	
	/**** check field size ******/
      ANAEVE:
	switch(ehid){
	case 0x0001: /* Block End Event */
/**SCALER*******************************************************************/
	  if(numofevent!=eventcount){
	    printf("%s:b %d:e %d:Inconsistent event header ID.\n"
		   ,comname,block,eventcount);
	    printf(" It indicates block end event, ");
	    printf("while it is not the last one.\n");
	    write_event();
	    exit(-1);
	  }
	  if(fieldsize!=FSIZSC){
	    printf("%s:b %d:e %d:Inconsistent field size.(0x%4x)\n"
		   ,comname,block,eventcount,fieldsize);
	    write_event();
	    printf("             Change fieldsize to FSIZSC.\n");
	    fieldsize=FSIZSC;
	  }
	  if((buff[wp]&0xf000)!=SCALEV){
	    printf("%s:b %d:e %d:Inconsistent event ID.(0x%4x)\n"
		   ,comname,block,eventcount,buff[wp]);
	    write_event();
	    printf("             Change eventid to SCALEV.\n");
	    buff[wp]=((buff[wp]&0x0fff)|SCALEV);
	  }
	  /**** read regions *******/
	  while(fieldsize>fsizecount){		
	    data=buff[++wp];
	    regionid=(data>>12);
	    regionsize=(data & 0x0fff);
	    if(regionid!=SCALER){
	      printf("%s:b %d:e %d:Wrong regionid (%x,%4x).\n",
		     comname,block,eventcount,regionid,regionsize);
	      printf("             Change regionid to SCALER.\n");
	      regionid=SCALER;
	    }
	    if(regionsize!=RSIZSC){
	      printf("%s:b %d:e %d:Wrong region size (%x,%4x).\n",
		     comname,block,eventcount,regionid,regionsize);
	      regionsize=RSIZSC;
	      printf("             Change region size to RSIZSC.\n");
	    }
	    camac_scaler();
	    fsizecount+=(regionsize+1);
	  }
	  break;
	case 0x0000:  /* DATA event */
/**BLPEVENT****************************************************************/
	  if(numofevent==eventcount){
	    printf("%s:b %d:e %d:Inconsistent event header ID.\n"
		   ,comname,block,eventcount);
	    printf(" It indicates data event, ");
	    printf("while it is the last one in the block.\n");
	    write_event();
	    exit(-1);
	  }
	  switch(fieldsize){
	  case FSIZEV:
	    if(!(buff[wp]&0x3000)){
	      printf("%s:b %d:e %d:Inconsistent event ID. (0x%4x)\n"
		   ,comname,block,eventcount,buff[wp]);
	      write_event();
	      printf("%s:b %d:e %d:Skip this event.\n\n",
		     comname,block,eventcount);
	      fsizecount+=(fieldsize-2);
	      wp+=(fieldsize-2);
	    }
	    break;
	  case FSIZDE:
	    if((buff[wp]&0x3000)!=0x3000){
	      printf("%s:b %d:e %d:Inconsistent event ID. (0x%4x)\n"
		     ,comname,block,eventcount,buff[wp]);
	      write_event();
	      printf("%s:b %d:e %d:Skip this event.\n\n",
		     comname,block,eventcount);
	      fsizecount+=(fieldsize-2);
	      wp+=(fieldsize-2);
	    }
	    break;
	  default:
	    printf("%s:b %d:e %d:Inconsistent Field Size! read:0x%4x\n"
		   ,comname,block,eventcount,fieldsize);
	    write_event();
	    switch(buff[wp]&0xf000){
	    case BLP1EV:
	    case BLP2EV:
	      printf("   Change to single BLP evnet.\n");
	      fieldsize=FSIZEV;
	      break;
	    case BLPDEV:
	      fieldsize=FSIZDE;
	      printf("   Change to double BLP evnet.\n");
	      break;
	    default:
	      printf("%s:b %d:e %d:Wrong event ID! (0x%4x)\n"
		     ,comname,block,eventcount,buff[wp]);
	      break;
	    }
	    printf("\n");
	    break;
	  }
	  /**** read regions *******/
	  while(fieldsize>fsizecount){		
	    data=buff[++wp];
	    regionid=(data>>12);
	    regionsize=(data & 0x0fff);

	    if(regionsize!=RSIZEV){
	      printf("%s:b %d:e %d:Wrong region size (%x,%4x).\n",
		     comname,block,eventcount,regionid,regionsize);
	      write_event();
	      printf("             Change region size to RSIZEV.\n\n");
	      regionsize=RSIZEV;
	    }

	    if(resttdc){
/**TDC****************************************************************/
	      if(regionid!=TDC){
		printf("%s:b %d:e %d:Wrong region ID (%x,%4x).\n",
		       comname,block,eventcount,regionid,regionsize);
		write_event();
		printf("             Change region size to TDC.\n\n");
		regionid=ADC;
	      }
	      tdcread();
	      resttdc--;
	    }
	    else{ 
/**ADC****************************************************************/
	      if(regionid!=ADC){
		printf("%s:b %d:e %d:Wrong region ID (%x,%4x).\n",
		       comname,block,eventcount,regionid,regionsize);
		write_event();
		printf("             Change region size to ADC.\n\n");
		regionid=ADC;
	      }
	      tdcread();
	      resttdc++;
	    }
/*********************************************************************/
	    fsizecount+=(regionsize+1);
	  }
	  break;
	default:
	  printf("%s:b %d:e %d:Wrong Event header ID (0x%4x).\n",
		 comname,block,eventcount,ehid);
	  write_event();
	  if(numofevent==eventcount){
	    printf(" Change to the Block End Event.\n\n");
    	    ehid=0x0001;
	  }
	  else{
	    printf(" Change to the Data Event.\n\n");
    	    ehid=0x0000;
	  }
	  goto ANAEVE;
	  break;
	}
	esizecount+=fieldsize;	

/**Analyzing****************************************************************/

/**Analyzing****************************************************************/

/**Booking******************************************************************/
#include "booking_blp.c"
/* Booking******************************************************************/
/*********************************************************************/
	if(fieldsize!=fsizecount){
	  printf("%s:b %d:e %d:Inconsistent Field Size Count! "
		 ,comname,block,eventcount);
	  printf("read:0x%4x count:0x%4x\n",fieldsize,fsizecount);
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
	printf("%s:b %d:e %d:Inconsistent Event Size Count! "
	       ,comname,block,eventcount);
	printf("read:0x%4x count:0x%4x\n",eventsize,esizecount);
	bsizecount+=(esizecount-eventsize);
	eventsize=esizecount;
	write_event();
	printf("    Use count value.\n");
	printf("\n");
      }
    }
/**********************************************************************
 Block trailer Detection
 **********************************************************************/
    if(buff[++wp]==0xffef){
      int i;
      bsizecount+=buff[++wp];
      if(bsizecount!=blocksize){
	printf("b %d:e %d:Inconsistent Block Size! read:%d count:%d\n",
	       block,eventcount,blocksize,bsizecount);
	exit(-1);
      }
      if(eventcount!=numofevent) {
	printf("b %d:e %d:Inconsistent Number of Events! read:%d count:%d\n",
	       block,eventcount,numofevent,eventcount);
	exit(-1);
      }
      /*	printf("WORDOFBLK-wp = %d\n",WORDOFBLKD-wp);*/
      /*	for(i=0;i<WORDOFBLK-wp;i++)
		printf("%d ",buff[wp+i]);
		printf("\n%h\n",wp+i);*/

    }
    else {
      printf("b %d:%d:I cannnot find block trailer in the end of bolck!\n",
	     block,buff[wp]);
      exit(-1);
    }
  }
}

void file_end(){
  int i,j;
  if(flag & 0x10) tcp_close();
  else close(fd);
  free((void *)rawbuff);free((void *)buff); /* Free the buffer */
  printf("I have read %d bytes.\n",totalbyte);
  printf("Block Analyzed:%d\tSkipped:%d\n",block-blockskip,blockskip);
  printf("Event Analyzed:%d\n",event);
#ifdef _LINUX
  if(flag & 0x8) hstend_g__();
  else hstend_f__();
#else
  if(flag & 0x8) hstend_g_();
  else hstend_f_();
#endif
  printf("CPU time = %f sec\n",clock()/1000000.);
  
  printf("SCALER\n");
  for(i=0;i<16;i++){
/*    printf("SCALER %x:",i);*/
    for(j=0;j<N_SCA;j++) printf("%12u",scaler[j][i]);
    printf("\n");
  }
  printf("SCALER\n");

  if(block-blockskip){
    printf("\nAnalyzing ratio: %5.2f\n",100.*(block-blockskip)/block);
  }
  exit(0);
}

void write_event(){
  int j;
  for(j=eventrp;j<eventrp+eventsize+EHSIZE+1;j++)
    printf("%4x ",buff[j]);
  printf("\n");
}
