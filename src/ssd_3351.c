/* 98/05/22 SSD-ball routine */

#include "ana10.h"
#include <stdio.h>

extern char comname[];
extern int rp,wp;
extern unsigned short regionsize,*buff;
extern unsigned long block;
extern int eventcount;
extern struct ssddata grssd;

int ssd_3351(){
  int i;
  unsigned short data;
  int validc;       /* valid data counter */
  int module_id,fera_id;
  unsigned short pw; /* pattern word */
  unsigned short  channel;       /* hit channel */
  
  rp=wp+regionsize;
  while(wp<rp){
    data=buff[++wp];
    if((data>>12)==0x8) {  /* if 3351 header word */
      validc=(data&0xf00)>>8;
      module_id=(data&0x00ff);
      pw=buff[++wp];

      for(i=0;i<validc;i++){ /* read channel data */
	channel=(buff[++wp]&0x7000)>>12;
	if(!((1<<channel)&pw)) { /* if inconsistent pattern word is detected */
	  printf("%s:b %ld:e %d:Inconsistent 3351 patten word.\n",
		 comname,block,eventcount);
	  wp=rp;
	  return(-1);
	}
	grssd.adc[grssd.k_adc]=(buff[wp]&0x0fff);
	fera_id=module_id&0xf;
	grssd.adc_ch[grssd.k_adc]=channel+(fera_id-1)*8;
	/*	printf("%02d  %02d  %5d\n",
	       grssd.k_adc,
	       grssd.adc_ch[grssd.k_adc],
	       grssd.adc[grssd.k_adc]);*/
	grssd.k_adc++;

      }
    }
    else { /* if no 3351 header word exists */
      printf("%s:b %ld:e %d:No 3351 header word exists.(%x)\n",
	     comname,block,eventcount,data);
      for(i=-16;i<16;i++) printf("%4x ",buff[wp+i]);
      fflush(stdin);
      wp=rp;
      return(-1);
      
    }
  }
  return(0);
}
