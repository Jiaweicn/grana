/* mwpcpcos2.c */
/* for ana_v1.5 or later 98/05/06 */

#include <stdio.h>
#include "ana10.h"
#include <vectcalc.h>

extern int rp,wp;
extern unsigned short regionsize,*buff;
extern unsigned long block;
extern int eventcount;
extern int pcoserror,pc_noch,pc_badcham,pc_hberr;
extern struct mwpcdata pc_x1,pc_x2,pc_u3,pc_v3,pc_u4,pc_v4;
extern char comname[];
extern struct mwpcdata *gr_mwpc[]; /* pointer for GR MWPC data */

int mwpc_pcos() {
  short widthdata,chamber_id; /* for PCOS only */
  unsigned short data;
  int i;

  widthdata=1;

  rp=wp+regionsize;
  data=buff[++wp];
  if((data & 0x0fff)!=(regionsize-1)){
    printf("%s:Inconsistent word count!(MWPC_PCOS)\n",comname);
    printf("%x\n",data);
  }
    
  while(rp>wp){
    data=buff[++wp];
    if((data & 0xc000)==0xc000) {/* if delimeter */
      if(widthdata!=1){
	printf("%s:No channel data!(MWPC) (b:%ld e:%d)\n",
	       comname,block,eventcount);
	pc_noch++;
      }
      widthdata=1;
      continue;
    }

    switch(data & 0x8000) {
    case 0x8000: /* if width */
      if(widthdata!=1){
	printf("%s:No channel data!(MWPC) %d(b:%ld e:%d)\n",
	       comname,widthdata,block,eventcount);
	pc_noch++;
	widthdata=1;
      }
      widthdata=(0x000f & data);
      break;
    case 0x0000: /* if channel data */
      chamber_id=(data & 0x7800);
      switch(chamber_id){
      case MWPC_X1:
	pcread(&pc_x1,data,widthdata);break;
      case MWPC_X2:
	pcread(&pc_x2,data,widthdata);break;
      case MWPC_U3:
	pcread(&pc_u3,data,widthdata);break;
      case MWPC_V3:
	pcread(&pc_v3,data,widthdata);break;
      case MWPC_U4:
	pcread(&pc_u4,data,widthdata);break;
      case MWPC_V4:
	pcread(&pc_v4,data,widthdata);break;
      case MWPC_X3: /* dummy 0pol */
	if(data & 0x7fc) break;
      default:
/*	printf("%s:Inconsistent ChamberID!(MWPC) %x(b:%ld e:%d)\n",
	       comname,chamber_id,block,eventcount);*/
	pc_badcham++;
	break;
      }
      widthdata=1;
      break;
    default:
      printf("%s:Inconsistent data!(MWPC) %x(b:%ld e:%d)\n",
	     comname,data,block,eventcount);
      break;
    }
  }

  for(i=0;i<6;i++){
    if(gr_mwpc[i]->wire>=PCEVE_L)
      printf("%s:Event buffer over flow!(%s) %d(b:%ld e:%d)\n",
	     comname,gr_mwpc[i]->label,gr_mwpc[i]->wire,block,eventcount);
  }
  return(0);
}


/* sub-routine *************************/
void pcread(struct mwpcdata *mwpc,unsigned short pcdata,short pcwidth){
  short module_id,channel,halfbit;

  module_id=(pcdata & 0x07c0)>>6;
  channel=module_id*32+((pcdata &0x003e)>>1);
  halfbit=(pcdata &0x0001);

  if((halfbit==0 && (pcwidth%2)==1) || 
     (halfbit==1 && (pcwidth%2)==0)){ mwpc->hberror=0;}
  else{
    pcoserror++;pc_hberr++;
    mwpc->hberror=1;
  }
  mwpc->channel[mwpc->wire]=channel;
  mwpc->width[mwpc->wire]=pcwidth;
  mwpc->halfbit[mwpc->wire]=halfbit;
  mwpc->wire++;
}

int mwpcana(struct mwpcdata *mwpc,struct chamber *cham){
  int i;

  if(mwpc->wire<10) mwpc->posid[++mwpc->posid[0]]=10+mwpc->wire;
  else mwpc->posid[++mwpc->posid[0]]=20;

  if(mwpc->hberror) mwpc->posid[++mwpc->posid[0]]=2;
  else{
    for(i=0;i<mwpc->wire;i++){
    if(mwpc->halfbit[i]==1) mwpc->pos[i]=mwpc->channel[i]*2+1;
    else mwpc->pos[i]=mwpc->channel[i]*2;
    }
    if(mwpc->wire==1) {
      mwpc->good=1;
      vectconb(1.0,cham->position,
	       (double)mwpc->pos[0],cham->displace,mwpc->posv);
      rotate_y(mwpc->tilt_s,mwpc->tilt_c,mwpc->posv,mwpc->rposv);
    }
    else mwpc->good=0;
  }
  return(0);
}


