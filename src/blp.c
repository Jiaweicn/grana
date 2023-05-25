/* blp.c
   00/01/06 Ver.1.0 Newly created for E137
*/
#include <stdio.h>
#include "ana10.h"

extern int rp,wp;
extern unsigned long block;
extern unsigned short regionsize,*buff;
extern unsigned short adc[16],tdc[16];
extern short eventid[16];
extern int eventcount;

int adcread() {
  int i=0;
  if(eventid[13]) i=8;
  if(regionsize!=8){
    printf("b %ld:e %d:Inconsistent adc regionsize! (0x%4x)\n"
	   ,block,eventcount,regionsize);
    regionsize=8;
/*    exit(-1);*/
  }
  rp=wp+regionsize;
  while(wp<rp){
    adc[i++]=buff[++wp];
  }
  return(0);
}

int tdcread() {
  unsigned short channel;
  if(regionsize!=8){
    printf("b %ld:Inconsistent tdc regionsize! (0x%4x)\n",block,regionsize);
    regionsize=8;
/*    exit(-1);*/
  }
  rp=wp+regionsize;
  while(wp<rp){
    channel=(buff[++wp]>>12);
    tdc[channel]=buff[wp]&0x0fff;
  }
  return(0);
}
