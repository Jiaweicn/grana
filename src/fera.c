/* Fera.c
   00/01/19 add LAS routine    *************************/

#include <stdio.h>
#include "ana10.h"

extern unsigned short regionsize,*buff;
extern int wp,rp;
extern unsigned long block;
extern int eventcount;
extern short fera_gr[],feret_gr[],fera_las[],feret_las[];
extern struct ssddata grssd;
extern char comname[];

int fera(){
  unsigned short data,counter;
  short gr,module_id,base_ch,channel;
  short *cfera;
  int k;
  rp=wp+regionsize;
  while(wp<rp){
    data=buff[++wp];
    if(data&0x8000){/* if FERA header word */
      gr=(data & 0x30)>>4;
      counter=(data>>11)&0xf;
      if(counter==0) counter=16;
      module_id=(data & 0xf);
    }
    else { /* unless FERA header word */
      printf("%s:No FERA header word exists. b:%ld e:%d\n",
	     comname,block,eventcount);
      wp=rp;
      return(-1);
    }
      
    switch(gr){
    case GR_FERA:   /** if GR-event **/
      cfera=fera_gr;
      break;
    case LAS_FERA:   /** if LAS-event **/
      cfera=fera_las;
      break;
    default:  /** if Wrong event **/
      printf("Wrong event!?(FERA:%x)\n",data);
      wp=rp;
      return(-1);
    }

    switch(module_id){
    case 0x01: /** if scinti or hodoscope Y **/
      base_ch=0;break;
    case 0x02: /** if hodoscope X **/
      base_ch=16;break;
    default:
      if(module_id<8) base_ch=(module_id-1)*16;
      else {
	printf("%s:Wrong module-id.(FERA)"
	       "b:%ld e:%d\n",comname,block,eventcount);
	wp=rp;
	return(-1);
      }
      break;
    }

    if(rp!=wp+counter){
      printf("%s:Inconsistent wordcount(FERA) b:%ld e:%d\n"
	     ,comname,block,eventcount);
      printf("\t");
      for(k=rp-regionsize;k<=rp;k++)
	printf("%4x ",buff[k]);
      printf("\n");
      wp=rp;
      return(-1);
    }
    for(k=2;k<=regionsize;k++) {
      data=buff[++wp];
      channel=base_ch+((data & 0x7800)>>11);
      cfera[channel]=(data & 0x07ff);
      /*    printf("channel:%x data:%x\n",
	    channel,fera_gr[channel]);*/
    }
  }
  return(0);
}


int feret(){
  unsigned short data,counter;
  short gr,fera_id,base_ch,channel;
  int k;

  rp=wp+regionsize;
  while(wp<rp){
    data=buff[++wp];
    if(data&0x8000) { /* if FERET header word */
      gr=(data & 0x30)>>4;
      counter=(data &0x7800)>>11;
      if(counter==0) counter=16;
      fera_id=(data & 0xf);
    }
    else { /* if no FERET header word exists */
      printf("%s:No FERET header word exists. b:%ld e:%d\n",
	     comname,block,eventcount);
      wp=rp;
      return(-1);
    }

    if(rp!=wp+counter){
      printf("%s:Inconsistent wordcount(FERET) b:%ld e:%d\n"
	     ,comname,block,eventcount);
      printf("\t");
      for(k=rp-regionsize;k<=rp;k++)
	printf("%4x ",buff[k]);
      printf("\n");
      wp=rp;
       return(-1);
    }
    
    base_ch=(fera_id-1)*16;
    switch(gr){
    case GR_FERA:
      for(k=0;k<counter;k++) {
	data=buff[++wp];
	channel=base_ch+((data & 0x7800)>>11);
	feret_gr[channel]=(data & 0x07ff);
      }
      break;
    case LAS_FERA:
      for(k=0;k<counter;k++) {
	data=buff[++wp];
	channel=base_ch+((data & 0x7800)>>11);
	feret_las[channel]=(data & 0x07ff);
      }
      break;
    case SSD_FERA:
      for(k=0;k<counter;k++) {
	data=buff[++wp];
	grssd.tdc[grssd.k_tdc]=(data&0x7ff);
	grssd.tdc_ch[grssd.k_tdc]=base_ch+((data & 0x7800)>>11);

#if 0
	if(base_ch==16)printf("%2d: %4d\n",grssd.tdc_ch[grssd.k_tdc],grssd.tdc[grssd.k_tdc]);
#endif
	grssd.k_tdc++;
      }
      break;
   default:
      printf("%s:Wrong module_id(FERET) b:%ld e:%d\n",
	     comname,block,eventcount);
      wp=rp;
      return(-1);
      break;
    }
  }
/*  printf("rp:%d wp:%d\n",rp,wp);
  if(rp!=wp)
    for(k=-16;k<16;k++)printf("%4x ",buff[wp+k]);*/
  return(0);
}


