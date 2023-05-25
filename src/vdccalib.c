/* vdccalib.c
            99/06/13   change format of VDC pos-time table */
/* 00/12/07 Change FHSIZE to FIHSIZE to compile on AIX     */
/*          Add _AIX to avoid bite swap on AIX             */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>



#include "ana10.h"
extern unsigned short *buff,*rawbuff;
extern size_t SIZEOFBLK;
#define FNLEN 30

void vdc_calb(int fd)
{
  int readbyte,rp,wp,i;
  FILE *file; /* for open time table */
  
  int blocksize,numofevent,fieldsize,fsizecount;
  int eventsize,numoffield,eventcount,fieldcount,bsizecount,esizecount;
  unsigned short regionid,regionsize;
  
  unsigned long  block,blockskip,event,eventerror,regionerror;
  unsigned long  totalbyte;
  
  char s[FNLEN];
  /************* Event buffer ***********/
  
  long fr_x=0; /* for counting VDC-event */
  long fr_u=0;
  long re_x=0;
  long re_u=0;
  
  long fr_x_count[L_3377]; /* for counting event */
  long fr_u_count[L_3377];
  long re_x_count[L_3377];  
  long re_u_count[L_3377];

  long fr_x_dpos[L_3377]; /* for counting event */
  long fr_u_dpos[L_3377];
  long re_x_dpos[L_3377];  
  long re_u_dpos[L_3377];
  
  int fr_x_l=-1,fr_x_h=-1;  /* for drift_start_ch and drift_end_ch */
  int fr_u_l=-1,fr_u_h=-1;
  int re_x_l=-1,re_x_h=-1;
  int re_u_l=-1,re_u_h=-1;
  
  
  for(i=0;i<L_3377;i++){
    fr_x_count[i]=0;
    fr_u_count[i]=0;
    re_x_count[i]=0;
    re_u_count[i]=0;
    
    fr_x_dpos[i]=0;
    fr_u_dpos[i]=0;
    re_x_dpos[i]=0;
    re_u_dpos[i]=0;
  }
  
  printf("We start VDC-calibration run!\n");
  block=0;blockskip=0;event=0;eventerror=0;regionerror=0;
  totalbyte=0;
  
  while(fr_x_l<0 || fr_x_l>L_3377){
    printf("GR_FR_X lower ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    fr_x_l=atoi(s);}
  
  while(fr_x_h<fr_x_l || fr_x_h>L_3377){
    printf("GR_FR_X higher ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    fr_x_h=atoi(s);}
  
  while(fr_u_l<0 || fr_u_l>L_3377){
    printf("GR_FR_U lower ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    fr_u_l=atoi(s);}
  
  while(fr_u_h<fr_u_l || fr_u_h>L_3377){
    printf("GR_FR_U higher ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    fr_u_h=atoi(s);}
  
  while(re_x_l<0 || re_u_l>L_3377){
    printf("GR_RE_X lower ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    re_x_l=atoi(s);}
  
  while(re_x_h<re_x_l|| re_u_h>L_3377){
    printf("GR_RE_X higher ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    re_x_h=atoi(s);}
  
  while(re_u_l<0 || re_u_l>L_3377){
    printf("GR_RE_U lower ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    re_u_l=atoi(s);}
  
  while(re_u_h<re_u_l || re_u_h>L_3377){
    printf("GR_RE_U higher ch ?:");
#if 0
    gets(s);
#endif
    fgets(s,FNLEN,stdin);
    re_u_h=atoi(s);}

  printf("GR_FR_X l:%d,h:%d\n",fr_x_l,fr_x_h);
  printf("GR_FR_U l:%d,h:%d\n",fr_u_l,fr_u_h);
  printf("GR_RE_X l:%d,h:%d\n",re_x_l,re_x_h);
  printf("GR_RE_U l:%d,h:%d\n",re_u_l,re_u_h);
  printf("Hit any key\n");
  getchar();
  
 newblock:
  while(1){
    readbyte=read(fd,rawbuff,SIZEOFBLK);
    /*    printf("readbytes = %d \n",readbyte);*/
    if(readbyte==-1) {
      printf("Error(read file)!\n");
      close(fd);
      exit(-1);
    }
    if(readbyte==0) {    /* file end */
      int j;
      printf("File end!\n");
      close(fd);

/************** writing table to the file *******************/
      file=fopen(FXTBL,"w");     
      if(file==NULL) fopenerror(FXTBL);
      fprintf(file,"%d\t%d\n",fr_x_l,fr_x_h);
      for(j=fr_x_h;j>=fr_x_l;j--){
	fr_x_dpos[j]=fr_x_dpos[j+1]+fr_x_count[j];
	fprintf(file,"%d\t%le\t%le\n",j,
		(double)fr_x_dpos[j]/fr_x*DMAX,
		(double)fr_x_count[j]/fr_x*DMAX);
      }
      fclose(file);

      file=fopen(FUTBL,"w");
      if(file==NULL)fopenerror(FUTBL);
      fprintf(file,"%d\t%d\n",fr_u_l,fr_u_h);
      for(j=fr_u_h;j>=fr_u_l;j--){
	fr_u_dpos[j]=fr_u_dpos[j+1]+fr_u_count[j];
	fprintf(file,"%d\t%le\t%le\n",j,
		(double)fr_u_dpos[j]/fr_u*DMAX,
		(double)fr_u_count[j]/fr_u*DMAX);
      }
      fclose(file);

      file=fopen(RXTBL,"w");
      if(file==NULL)fopenerror(RXTBL);
      fprintf(file,"%d\t%d\n",re_x_l,re_x_h);
      for(j=re_x_h;j>=re_x_l;j--){
	re_x_dpos[j]=re_x_dpos[j+1]+re_x_count[j];
	fprintf(file,"%d\t%le\t%le\n",j,
		(double)re_x_dpos[j]/re_x*DMAX,
		(double)re_x_count[j]/re_x*DMAX);


      }
      fclose(file);

      file=fopen(RUTBL,"w");
      if(file==NULL)fopenerror(RUTBL);
      fprintf(file,"%d\t%d\n",re_u_l,re_u_h);
      for(j=re_u_h;j>=re_u_l;j--){
	re_u_dpos[j]=re_u_dpos[j+1]+re_u_count[j];
	fprintf(file,"%d\t%le\t%le\n",j,
		(double)re_u_dpos[j]/re_u*DMAX,
		(double)re_u_count[j]/re_u*DMAX);
      }
      fclose(file);
/************** writing table to the file *******************/

      printf("I have read %ld bytes.\n",totalbyte);
      printf("Block Analyzed:%ld\tSkipped:%ld\n",block-blockskip,blockskip);
      printf("Event Analyzed:%ld\n",event);
      printf("              GR_FR_X:%ld\n",fr_x);
      printf("              GR_FR_U:%ld\n",fr_u);
      printf("              GR_RE_X:%ld\n",re_x);
      printf("              GR_RE_U:%ld\n",re_u);
      printf("CPU time = %f sec\n",clock()/1000000.);
      exit(0);
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
      if(buff[++wp]!=BHSIZE) 
	printf("Inconsistent Block Header Size! : %d word\n",buff[wp]);
      switch(buff[++wp]){
      case 0x0f01:		/* if Run Start Block*/
	printf("#################################\n");
	printf("Run Start Block.(%ld)\n",block);
	blocksize=buff[++wp];wp+=2;

	if(buff[wp+1]==0) ++wp; /* Event flag*/
	++wp;			/* Data Format Version */

#ifndef _AIX
	swab(&rawbuff[++wp],&timedata,sizeof(int));
#else
	timedata=rawbuff[++wp];
#endif	

	/*	printf("byteorder %x\n",timedata); */	
	++wp;

	timedata=ntohl(*(int *)&rawbuff[++wp]);
	printf("%s",ctime(&timedata)); /*tmp*/
	++wp;			/* time stamp */

	printf("run%d\t",buff[++wp]); /* run # */
	printf("%s\n",(char *)&rawbuff[++wp]); /* comment */
	printf("#################################\n");
	block++;

	goto newblock;

      case 0x0f02:		/* if Run End Block */
	printf("Run End Block.(%ld)\n",block);
	blocksize=buff[++wp];
	wp++;			/* Block # */
	wp++;			/* event # */

	if(buff[wp+1]==0) ++wp; /* Event flag*/
	++wp;			/* Data Format Version */

#ifndef _AIX
	swab(&rawbuff[++wp],&timedata,sizeof(int));
#else
	timedata=rawbuff[++wp];
#endif	
				
	/*	printf("byteorder %x\n",timedata); */	
	++wp;

	timedata=ntohl(*(int *)&rawbuff[++wp]);
	/*	printf("%s",ctime(&timedata));*/
	++wp;			/* time stamp */

	printf("run %d\t",buff[++wp]); /* run # */
	printf("%s\n",(char *)&rawbuff[++wp]); /* comment */
	goto newblock;

      case 0x0000:		/*if Data Block*/
	if((block & 0xff)==0xff) 
	  printf("Data Block : %ld th\n",block);
	blocksize=buff[++wp];	break;

      default:
	printf("Inconsistent Block ID : %x\n",buff[wp]);
	blocksize=buff[++wp];
	break;
      }

      if(buff[++wp]!=block) {
	printf("Inconsistent Block Number! read:%d th  count:%ld th\n",
	       buff[wp],block);
	while(buff[wp]!=block) {
/*	  printf("I found a skipped block!\n");*/
	  blockskip++;block++;
	  if(buff[wp]<block) {
	    printf("Fetal error!\t buff[wp] < block \n");
	    exit(-1);}
	}
      }
      numofevent=buff[++wp];
      block++;
      

/**********************************************************************
 Event Header Detection
**********************************************************************/
      while(blocksize>bsizecount+2 && numofevent>eventcount){

	int i;


	/**********************************************/

	if(buff[++wp]==0xffdf) {
	  esizecount=0;fieldcount=0;
	  event++;eventcount++;
	  if(buff[++wp]!=EHSIZE) 
	    printf("Inconsistent Event Header Size! : %d words\n",buff[wp]);
	  bsizecount+=buff[wp];
	  switch(buff[++wp]){
	  case 0x0001:
	    /*	    printf("Block End Event. : %d th\n",event);*/
	    break;
	  case 0x0000:
	    /*	    printf("Data Event : %d th\n",event);*/
	    break;
	  default:
	    printf("Inconsistent Event ID : %x\n",buff[wp]);
	  }

	  eventsize=buff[++wp];
	  bsizecount+=eventsize;
	  if(buff[++wp]!=eventcount-1)
	    printf("Inconsistent Event Number! read:%dth count:%dth\n",
		   buff[wp]+1,eventcount);
	  if(buff[++wp]!=1)
	    printf("Inconsistent Number of Fields! %d\n",buff[wp]);
	  numoffield=buff[wp];

/**********************************************************************
   Field Header Detection
**********************************************************************/

	  while(eventsize>esizecount && numoffield > fieldcount){
	    unsigned short data;
	    
	    if(buff[++wp]==0xffcf) {
	      fsizecount=0;
	      if(buff[++wp]!=FIHSIZE) 
		printf("Inconsistent Field Header Size! :%d words\n",buff[wp]);
	      esizecount+=buff[wp];fieldcount++;
	      if(buff[++wp]!=0x0000) 
		printf("Inconsistent Field ID! :%x\n",buff[wp]);
	      fieldsize=buff[++wp];
	      esizecount+=fieldsize;

/*********************************************************************/
/*********************************************************************/
/* region function is here*/
/*********************************************************************/
/*********************************************************************/
	      while(fieldsize>fsizecount){		
		int mod_fr_x,mod_fr_u,mod_re_x,mod_re_u;
		short module_id,channel,base_ch,time;
		unsigned short chamber_id;
		
		data=buff[++wp];
		regionid=(data>>12);
		regionsize=(data & 0x0fff);
		fsizecount+=(regionsize+1);

		switch(regionid) { 
/**3377VDC******************************************************************/
		case VDC_3377:
		  rp=wp+regionsize;
		  chamber_id=0;

		  while(rp>wp){
		    data=buff[++wp]; 
		    if(data>>15) { /* if 3377 header */
		      module_id=(data & 0x000f);
		      base_ch=module_id*32;
		      chamber_id=(data&0x81f0);}
		    else {	/* unless 3377 header */
		      channel=(data>>10)+base_ch;
		      time=(data & 0x03ff);

		      switch(chamber_id) {
		      case GR_FR_X:
			if(time>=fr_x_l && time<=fr_x_h) {
			  fr_x++;
			  fr_x_count[time]++;
			}
			break;
			
		      case GR_FR_U:
			if(time>=fr_u_l && time<=fr_u_h) {
			  fr_u++;
			  fr_u_count[time]++;
			}
			break;

		      case GR_RE_X:
			if(time>=re_x_l && time<=re_x_h) {
			  re_x++;
			  re_x_count[time]++;
			}
			break;

		      case GR_RE_U:
			if(time>=re_u_l && time<=re_u_h) {
			  re_u++;
			  re_u_count[time]++;
			}
			break;

		      default:
			printf
			  ("Inconsistent chamber_id(VDC_GR)! %x\n",chamber_id);
			wp=rp;
			break;
		      }
		    }
		  }
		  break;
		  wp+=regionsize;
		  break;		    

/**************************************************************************/
		default:
		  wp+=regionsize;
		  break;
		}
	      }
	    }
	    else 
	      printf("I cannot find fieldheader in the head of field!\n");

	    if(fieldsize!=fsizecount)
	      printf("Inconsistent Field Size! read:%d count:%d\n",
		     fieldsize,fsizecount);
	    
	  }
	    
	  if(fieldcount!=numoffield)
	    printf("Inconsistent Number of Fields! read:%d count:%d\n",
		   numoffield,fieldcount);
	  
	  if(eventsize!=esizecount) 
	    printf("Inconsistent Event Size! read:%d count:%d\n",
		   eventsize,esizecount);
	}
	else {
	  printf("I cannot find event header at the head of event!(%ld)\n",
		 block);
	  eventerror++;bsizecount++;
	  /*	  printf("I found %x in %d of wp.\n",buff[wp],wp);*/
	}
      }
/**********************************************************************
 Block trailer Detection
 **********************************************************************/
      if(buff[++wp]==0xffef){
	int i;
	bsizecount+=buff[++wp];
	if(bsizecount!=blocksize)
	  printf("Inconsistent Block Size! read:%d count:%d\n",
		 blocksize,bsizecount);
	if(eventcount!=numofevent) {
	  printf("Inconsistent Number of Events! read:%d count:%d\n",
		 numofevent,eventcount);
	}

      }
      else {
	printf("I cannnot find block trailer in the end of bolck!\n");
      }
    }
    else {
      printf("I cannot find blockheader in the head of block!\n");
    }
  }
}


void tableread(char *s,struct vdcdata *vdc){
  FILE *file;
  int i;
  int channel;
  int *l;
  int *h;
  double *table;
  double *derr;

  l=&vdc->time_l;
  h=&vdc->time_h;
  table=vdc->table;
  derr=vdc->derr;

  file=fopen(s,"r");
  if(file==NULL) fopenerror(s);
  if(fscanf(file,"%d\t%d\n",l,h)!=2){
    printf("File read error! (%s)\n",s);
    exit(-1);
  }
  if((*l<0 || *l>2023) || (*h>2023 || *h<*l)) {
     printf("%s:Table file contains a wrong line.\n",s);
     exit(-1);
   }

  for(i=*l;i<=*h;i++){
   if((fscanf(file,"%d",&channel)+
       fscanf(file,"%le%le\n",table+channel,derr+channel))!=3){
         printf("File read error! (%s)\n",s);
	 exit(-1);
       }
   if(*(derr+channel)<10e-10) *(derr+channel)=10e-10;
  }
#if 0
  /********* double diffrential ***********/
  derr[*l]=(1.0+table[*l+1])-2.0*table[*l];
  for(i=*l+1;i<*h;i++) derr[i]=(table[i-1]+table[i+1])-2.0*table[i];
  derr[*h]=table[*h-1]-2.0*table[*h];
  /********* double diffrential ***********/
#endif
  vdc->time_h--;  /* Exclude drift=0 wire */
  fclose(file);
}


void fopenerror(char *s){
  switch(errno){
  case EACCES:
    printf("%s:Permission denied!\n",s);break;
  case EISDIR:
    printf("%s is directory!\n",s);break;
  case ENOENT:
    printf("%s:No such a file!\n",s);break;
  default:
    printf("%s:Error(fffopen)! \n",s);break;
  }
  printf("Exit.\n");
  exit(-1);
}


void tableread2(int n,double *a, struct vdcdata *vdc){
  int i,j;
  double dt;
  int *l;
  int *h;
  double *table;
  double *derr;

  l=&vdc->time_l;
  h=&vdc->time_h;
  table=vdc->table;
  derr=vdc->derr;

  for(i=vdc->time_l;i<vdc->time_h+1;i++){
    dt=vdc->time_h-i;
    table[i]=0;
    for(j=0;j<n;j++){
      table[i]+=a[j]*pow(dt,(double)j);
      derr[i]=0.001;
    }
  }
}




