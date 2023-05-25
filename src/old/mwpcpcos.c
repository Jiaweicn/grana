#include <stdio.h>
#include "ana8.h"

extern int rp,wp;
extern unsigned short regionsize,*buff;
extern unsigned long block;
extern int eventcount,k_pc_x1,k_pc_x2,k_pc_u3,k_pc_v3,k_pc_u4,k_pc_v4;
extern int pcoserror,pc_noch,pc_badcham;
extern struct mwpc pc_x1,pc_x2,pc_u3,pc_v3,pc_u4,pc_v4;

int mwpc_pcos() {
  short widthdata,width,hberror,halfbit,chamber_id; /* for PCOS only */
  short module_id,channel;
  unsigned short data;

  width=0; /*tmp*/
  widthdata=1;

  rp=wp+regionsize;
  data=buff[++wp];
  if((data & 0x0fff)!=(regionsize-1)){
    printf("Inconsistent word count!(MWPC_PCOS)\n");
    printf("%x\n",data);
  }
  while(rp>wp){
    data=buff[++wp];
/*  printf("data:%x\n",data);*/

    if((data & 0xc000)==0xc000) {/* if delimeter */
      if(width==1){
	printf("No channel data!(MWPC) b:%d  e:%d\n",block,eventcount);
/*	pc_noch++;*//*tmp*/}
      widthdata=1;width=0;
/*    printf("delimiter word.\n");*/
      continue;}

    switch(data & 0x8000) {
    case 0x8000: /* if width */
      if(width==1){
	printf("No channel data!(MWPC) b:%d  e:%d\n",block,eventcount);
/*	pc_noch++;*//*tmp*/}
      widthdata=(0x000f & data);
      width=1;
      break;
    case 0x0000: /* if channel data */
      width=0;
      chamber_id=(data & 0x7800);
      module_id=(data & 0x07c0)>>6;
      halfbit=(data &0x0001);
      channel=module_id*32+((data &0x003e)>>1);

      if((halfbit==0 && (widthdata%2)==1) || 
	 (halfbit==1 && (widthdata%2)==0)){ hberror=0;}
      else{
	pcoserror++;
	hberror=1;}

      switch(chamber_id){
      case MWPC_X1:
	pc_x1.channel[k_pc_x1]=channel;
	pc_x1.width[k_pc_x1]=widthdata;
	pc_x1.halfbit[k_pc_x1]=halfbit;
	pc_x1.hberror=hberror;
	widthdata=1;
	k_pc_x1++;
	break;
      case MWPC_X2:
	pc_x2.channel[k_pc_x2]=channel;
	pc_x2.width[k_pc_x2]=widthdata;
	pc_x2.halfbit[k_pc_x2]=halfbit;
	pc_x2.hberror=hberror;
	widthdata=1;
	k_pc_x2++;
	break;
      case MWPC_U3:
	pc_u3.channel[k_pc_u3]=channel;
	pc_u3.width[k_pc_u3]=widthdata;
	pc_u3.halfbit[k_pc_u3]=halfbit;
	pc_u3.hberror=hberror;
	widthdata=1;
	k_pc_u3++;
	break;
      case MWPC_V3:
	pc_v3.channel[k_pc_v3]=channel;
	pc_v3.width[k_pc_v3]=widthdata;
	pc_v3.halfbit[k_pc_v3]=halfbit;
	pc_v3.hberror=hberror;
	widthdata=1;
	k_pc_v3++;
	break;
      case MWPC_U4:
	pc_u4.channel[k_pc_u4]=channel;
	pc_u4.width[k_pc_u4]=widthdata;
	pc_u4.halfbit[k_pc_u4]=halfbit;
	pc_u4.hberror=hberror;
	widthdata=1;
	k_pc_u4++;
	break;
      case MWPC_V4:
	pc_v4.channel[k_pc_v4]=channel;
	pc_v4.width[k_pc_v4]=widthdata;
	pc_v4.halfbit[k_pc_v4]=halfbit;
	pc_v4.hberror=hberror;
	widthdata=1;
	k_pc_v4++;
	break;
      default:
/*    printf("Inconsistent ChamberID!(MWPC) b:%d e:%d %x\n",
      block,eventcount,data);*/
	pc_badcham++;
	break;}
      break;
    default:
      printf("Inconsistent data!(MWPC) b:%d e:%d %x\n",block,eventcount,data);
      width=0;break;}
  }

  if(pc_x1.hberror) pc_noch+=k_pc_x1;
  if(pc_x2.hberror) pc_noch+=k_pc_x2;
  if(pc_u3.hberror) pc_noch+=k_pc_u3;
  if(pc_v3.hberror) pc_noch+=k_pc_v3;
  if(pc_u4.hberror) pc_noch+=k_pc_u4;
  if(pc_v4.hberror) pc_noch+=k_pc_v4;

  if(k_pc_x1>=PCEVE_L)
    printf("Event buffer over flow!(MWPX_X1) %d\n",k_pc_x1);
  if(k_pc_x2>=PCEVE_L)
    printf("Event buffer over flow!(MWPX_X2) %d\n",k_pc_x2);
  if(k_pc_u3>=PCEVE_L)
    printf("Event buffer over flow!(MWPX_U3) %d\n",k_pc_u3);
  if(k_pc_v3>=PCEVE_L)
    printf("Event buffer over flow!(MWPX_V3) %d\n",k_pc_v3);
  if(k_pc_u4>=PCEVE_L)
    printf("Event buffer over flow!(MWPX_U4) %d\n",k_pc_u4);
  if(k_pc_v4>=PCEVE_L)
    printf("Event buffer over flow!(MWPX_V4) %d\n",k_pc_v4);


/*****tmp*******/

/* for(i=0;i<k_pc_x1;i++)
   printf("MWPC_X1 \t channel:%d \t width:%d \t halfbit:%d\n",
   pc_x1.channel[i],pc_x1.width[i],pc_x1.halfbit[i]);
   for(i=0;i<k_pc_x2;i++)
   printf("MWPC_X2 \t channel:%d \t width:%d \t halfbit:%d\n",
   pc_x2.channel[i],pc_x2.width[i],pc_x2.halfbit[i]);
   for(i=0;i<k_pc_u3;i++)
   printf("MWPC_U3 \t channel:%d \t width:%d \t halfbit:%d\n",
   pc_u3.channel[i],pc_u3.width[i],pc_u3.halfbit[i]);
   for(i=0;i<k_pc_v3;i++)
   printf("MWPC_V3 \t channel:%d \t width:%d \t halfbit:%d\n",
   pc_v3.channel[i],pc_v3.width[i],pc_v3.halfbit[i]);
   for(i=0;i<k_pc_u4;i++)
   printf("MWPC_U4 \t channel:%d \t width:%d \t halfbit:%d\n",
   pc_u4.channel[i],pc_u4.width[i],pc_u4.halfbit[i]);
   for(i=0;i<k_pc_v4;i++)
   printf("MWPC_V4 \t channel:%d \t width:%d \t halfbit:%d\n",
   pc_v4.channel[i],pc_v4.width[i],pc_v4.halfbit[i]);
*/

/*	printf("\n");*/

  return(0);
}
