/*
Build$B8e$N%G!<%?$rFI$`%W%m%0%i%`(B
          Ver. 0.2  $B%G!<%?$r(BRegion$B$4$H$K@Z$jJ,$1$i$l$k(B
                    NIM-IN-BIT $B$N:F8=$K@.8y(B
97/07/20  Ver. 0.3  FERA,INPUT_REG $B$r%5%]!<%H(B
97/07/21  Ver. 0.31 FERET $B$r%5%]!<%H(B
97/07/23  Ver. 0.4  VDC3377 $B$r%5%]!<%H(B
97/07/23  Ver. 0.41 event buffer $B$N=i4|2=%W%m%;%9$r2~NI(B($BB.EY(B25%$B8~>e(B)
97/07/23  Ver. 0.5  MWPC $B$r%5%]!<%H(B obsoleted.
97/07/26  Ver. 0.6  MWPC $B$r%5%]!<%H(B
97/07/26  Ver. 0.7  Booking $B$N%?%$%_%s%0$rJQ99(B
97/08/01  Ver. 0.71 $B$$$m$$$mJQ99(B(ray trace $B$N0lJb<jA0$^$G(B)
                    -vdc $B%*%W%7%g%s$r:NMQ(B
97/08/06  Ver. 0.8  ray trace $B$r%5%]!<%H(B
          Ver. 0.9  Histgram-ID $B$NIU$1$+$((B
                    Vertex routine $B$r%5%]!<%H(B
97/09/12  Ver. 1.0  FPP $BJP6KB,Dj$r%5%]!<%H(B
                    VDC raytracing $B$r2~NI(B($B:G>.Fs>hK!$rF3F~(B)
97/09/26  Ver. 1.1  Histgram $B$r$d$dJQ99(B
                    run2061 $B$K:GE,2=(B
97/10/12  Ver. 1.2  Y-cut Background $B$rFsJ,3d(B
97/10/15  Ver. 1.3  Event ID $B$NDj5A$r(B C-style $B$KJQ99(B
                    VDC $B$N(B cluster$B?t$,A[DjCM$rD6$($?;~$KH/@8$9$k%P%0$r(Bfix
97/12/18  Ver. 1.4  $B%U%m!<@)8f!&(BVDC$B@0M}K!JQ99$N$?$a$N%3%s%Q%$%k%*%W%7%g%s$r@_Dj(B
                    hbfile name $B$N%P%0$r(Bfix
98/02/28            -global, -online, -port $B$N3F%*%W%7%g%s$r:NMQ(B
98/04/11            Ctrl-c $B$K$h$k%W%m%0%i%`$NCf;_=hM}$r%5%]!<%H(B
98/04/20            -b $B%*%W%7%g%s$r:NMQ(B
98/05/01  Ver. 1.5  $B#27n0JMh$N3+H/9`L\$r(BFPP$B$KBP$7$F$bM-8z$K(B
                    FPP$B2r@O$N$?$a$N%3%s%Q%$%k%*%W%7%g%s$r@_Dj(B
                    analib5.f $B$r(B analib6.f, histdef.f $B$KJ,3d(B
                    analyzing_v1.4.c $B$r(B analyzing_v1.5.c, correction.c $B$KJ,3d(B
98/05/12            MWPC $B$N%k!<%A%s$r2~NI(B
98/07/04  Ver. 1.51 FERA, FERET $B$N(B word count $B$r%A%'%C%/$9$k%k!<%A%s$rDI2C(B
                    3351PADC $B$N%k!<%A%s$rDI2C(B
98/07/30            $B=DJP6KMQ%3%s%Q%$%k%*%W%7%g%s(B -D_VER_POL $B$r@_Dj(B
99/03/24            Region Header $B$N07$$$r0lItJQ99(B
                    Unknown region $B$O(B case by case $B$KBP1~$9$k(B
		    Region f(Check Sum) $B$r%5%]!<%H(B
99/06/13  Ver. 1.52 VDC Least Kai-square $B%k!<%A%s$rJQ99(B
                    VDC time-pos table $B$N(B format $B$rJQ99(B
99/11/02  Ver. 1.53 VDC Reflect correction $B$rGQ;_(B
00/01/06  Ver. 1.54 _VDC_TWOWIRE_ON $B;HMQ;~$K(B vdc->dis $B$,(B $B!\J}8~$K(B tail $B$r(B
                    $B0z$/%P%0$r(Bfix
                    BLP $B$r%5%]!<%H(B
00/01/13  Ver. 1.55 Event header $BN`$N%(%i!<%a%C%;!<%8$r0lItJQ99(B
                    BLP $B$N%(%i!<%A%'%C%/$r4E$/$9$k%3%s%Q%$%k%*%W%7%g%s(B
                      make blp2 $B$rDI2C(B
00/01/13  Ver. 1.56 anablp.c $B$r(B v1.1 $B$K!#(B
                    Camac Scaler $B$N:GBgCM$r5,Dj$9$k(B SC_MAX $B$rDj5A(B
00/01/15  anasc  Ver.1.0
                    ana_v1.56 $B$+$iJ,N%!#(B
                    Scaler $B@lMQ%k!<%A%s$H$7$FITMWItJ,$r:o=|(B
00/12/07  anasc     Change FHSIZE to FIHSIZE to compile on AIX 
                    Add _AIX to avoid bite swap on AIX         
05/07/18            ana $B$H$N@09g@-$N$?$a$K(B grcr $B$NDj5A$rDI2C(B
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

int flag; /* option flag */

int blocksize,numofevent,fieldsize,fsizecount;
int eventsize,numoffield,eventcount,fieldcount,bsizecount,esizecount;
unsigned short regionid,regionsize;

size_t SIZEOFBLK,WORDOFBLK;
unsigned short *buff,*rawbuff;
unsigned long  block,blockskip,event,eventerror,regionerror;
unsigned long  totalbyte;
struct runparm grcr;

/************* Event buffer ***********/
short eventid[16];         /* for INPUT register */
short fcet_id[16];         /* for FCET-INPUT register */

unsigned int scaler[N_SCA][16];          /* for CAMAC scaler */
int scalerno;

void file_end();

int main(int argc,char *argv[])
{
  int i;
  struct sigaction act;       /* definition of interrupt routine */
  char hbfile[30];

  clock();


  strcpy(comname,argv[0]);
  flag=startup(argc,argv,&fd,hbfile);

  
  act.sa_handler=file_end;
  sigaction(SIGINT,&act,NULL);

/*** Initialize for CAMAC scaler ************/
  for(i=0;i<16;i++){
    scaler[0][i]=0;scaler[1][i]=0;
  }


  block=0;blockskip=0;event=0;eventerror=0;regionerror=0;
  totalbyte=0;

 newblock:
  while(1){
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
	buff=rawbuff;
#endif
    wp=0;

/******************************************************************
  Block Header Detection
 ******************************************************************/

    if(buff[wp]==0xffff){
      int i,timedata;
      
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
	    exit(-1);
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
}

/**********************************************/

	if(buff[++wp]==0xffdf) {
	  eventrp=wp;
	  esizecount=0;fieldcount=0;
	  event++;eventcount++;
	  if(buff[++wp]!=EHSIZE) 
	    printf("b %d:e %d:Inconsistent Event Header Size! : %d words\n",
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
	    printf("b %d:e %d:Inconsistent Event ID : %x\n",
		   block,eventcount,buff[wp]);
	  }

	  eventsize=buff[++wp];
	  bsizecount+=eventsize;
	  if(buff[++wp]!=eventcount-1)
	    printf("b %d:e %dInconsistent Event Number! read:%dth count:%dth\n"
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
	    short j,histid1,histid2; 
	    
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
/**SCALER*******************************************************************/
		case SCALER:
		  camac_scaler();
		  break;
/**************************************************************************/
		default:
		  wp+=regionsize;
		  break;
		}
	      }
/*********************************************************************/
	    }
	    else {
	      printf("I cannot find fieldheader in the head of field!\n");
	      exit(-1);
	    }
	    if(fieldsize!=fsizecount){
	      printf("%s:b %d:e %d:Inconsistent Field Size! read:%d count:%d\n"
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
	    printf("b %d:e %d:Inconsistent Event Size! read:%d count:%d\n",
		   block,eventcount,eventsize,esizecount);
	    exit(-1);
	  }
	}
        else {
	  printf("b %d:e %d:There is no event header at the head of event!\n",
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
    else {
      printf("b %d:I cannot find blockheader in the head of block!\n",
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
  printf("I have read %d bytes.\n",totalbyte);
  printf("Block Analyzed:%d\tSkipped:%d\n",block-blockskip,blockskip);
  printf("Event Analyzed:%d\n",event);
  printf("CPU time = %f sec\n",clock()/1000000.);
  
  for(i=0;i<16;i++){
    printf("SCALER %x:",i);
    for(j=0;j<N_SCA;j++) printf("%12u",scaler[j][i]);
    printf("\n");
  }

  if(block-blockskip){
    printf("\nAnalyzing ratio: %5.2f\n",100.*(block-blockskip)/block);
  }
  exit(0);
}

