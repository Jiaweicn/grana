/* vdc3388_v2.c */
/* ver0.2a   99/06/11 modify least squre routine */
/* ver0.2b   99/06/12 add routine to exclude worst wire */
/* ver0.2c   99/11/02 remove routine to exclude worst wire */
/* ver0.2d   00/01/06 change the way to decide minimum wire when posid=5 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "ana9.h"
#include "config.h"

extern int rp,wp;
extern unsigned short regionsize,*buff;
extern unsigned long block;
extern int eventcount;
extern struct vdcdata grfx,grfu,grrx,grru;
extern struct chamber vdc_fx,vdc_fu,vdc_rx,vdc_ru;
extern char comname[];

void vdcread(struct vdcdata *vdc,short channel,short time);
void vdcsort(struct vdcdata *vdc);
double leastsqrt(struct vdcdata *vdc);

int vdc_3377(){
  unsigned short chamber_id,data;
  short time,module_id,base_ch,channel;
  int mod_fr_x,mod_fr_u,mod_re_x,mod_re_u,k,i;

  rp=wp+regionsize;
  chamber_id=0;k=0;
  mod_fr_x=0;mod_fr_u=0;mod_re_x=0;mod_re_u=0;		  

  while(rp>wp){
    data=buff[++wp]; 
    if(data>>15) {  /* if 3377 header */
      module_id=(data & 0x000f);
      base_ch=module_id*32;
      chamber_id=(data & 0x81f0);
      switch(chamber_id){
      case GR_FR_X:
	if(module_id!=0) /* for GR_FR_X sorting */
	  mod_fr_x=1;
	break;
      case GR_FR_U:
	mod_fr_u=1;break;
      case GR_RE_X:
	mod_re_x=1;break;
      case GR_RE_U:
	mod_re_u=1;break;
      default:
/*	printf("%s:b %d:e %d:Inconsistent chamber_id(VDC_GR)! %x\n",
	       comname,block,eventcount,chamber_id);*/
	chamber_id=0xffff; /* indicate wrong chamber_id */
	wp=rp;
	break;
      }
    }

    else {          /* unless 3377 header */
      channel=(data>>10)+base_ch;
/*    printf("\tVDC channel:%d\tchannel:%d\tbase_ch:%d\n",
      channel,data>>10,base_ch);*/
      time=(data & 0x03ff);

      switch(chamber_id) {
      case GR_FR_X:
	if(module_id==0) mod_fr_x=1; /* for GR_FR_X sorting */
	vdcread(&grfx,channel,time);
	break;
      case GR_FR_U:
	vdcread(&grfu,channel,time);
	break;
      case GR_RE_X:
	vdcread(&grrx,channel,time);
	break;
      case GR_RE_U:
	vdcread(&grru,channel,time);
	break;
      case 0xffff:
	break;
      default:
	printf("%s:No 3377 header word exists.(GR_VDC) b:%d e:%d\n"
	       ,comname,block,eventcount);
/*	for(i=rp-regionsize;i<=rp;i++)
	  printf("%4x ",buff[i]);
	printf("\n");*/
	wp=rp;
	return(-1);
	break;
      }
    }
  }

  /*******sorting**************/
  if(mod_fr_x==1) vdcsort(&grfx);
  if(mod_fr_u==1) vdcsort(&grfu);
  if(mod_re_x==1) vdcsort(&grrx);
  if(mod_re_u==1) vdcsort(&grru);
  /*******sorting**************/

  return(0);
}

/****** sub-routine ************************************************/
void vdcread(struct vdcdata *vdc,short channel,short time){

#ifdef _VDC_MULT_HIT /* if VDC multi hit are allowed */
  if(vdc->wire){
    if(vdc->channel[vdc->wire-1]==channel) vdc->wire--;
  }
#endif /* _VDC_MULT_HIT */
  vdc->channel[vdc->wire]=channel;
  vdc->time[vdc->wire]=time;
/*  	printf("%s\t channel:%d\t time:%d\n",vdc->label,
	       vdc->channel[vdc->wire],vdc->time[vdc->wire]); */
  vdc->wire++;

  if(vdc->wire>=VDCEVE_L) 
    printf("Event buffer over flow!(%s) %d (%d:%d)\n",
	   vdc->label,vdc->wire,block,eventcount);
}

void vdcsort(struct vdcdata *vdc){
  int i;
  sort3377(vdc->channel,vdc->time,vdc->wire,vdc->label);
  for(i=0;i<vdc->wire;i++){
/*      printf("%s\t channel:%d\t time:%d\n",vdc->label,
    vdc->channel[i],vdc->time[i]);*/
    }
}

struct tnode {
  short tmp_ch;
  short tmp_time;
  struct tnode *left;
  struct tnode *right;
};

void sort3377(short channel[],short time[],int k,char *label) {
  struct tnode *tree(struct tnode *,short ch,short tm);
  void treep(struct tnode *,short channel[],short time[],int *i);

  int i;
  struct tnode *root;
  root=NULL;
  for(i=0;i<k;i++){
    root=tree(root,channel[i],time[i]);
  }
  i=0;
  treep(root,channel,time,&i);
}


struct tnode *tree(struct tnode *p,short ch,short tm)
{
  if(p==NULL){
    p=(struct tnode *)malloc(sizeof(struct tnode));
    p->tmp_ch=ch;p->tmp_time=tm;
    p->left=NULL;p->right=NULL;
  }
  else {
    if((p->tmp_ch)>ch) 
      p->left=tree(p->left,ch,tm);
   else
      p->right=tree(p->right,ch,tm);
  }
  return(p);
}


void treep(struct tnode *p,short channel[],short time[],int *i) {
  if(p!=NULL){
    treep(p->left,channel,time,i);
/*    printf("channel:%d time:%d(%x)\n",p->tmp_ch,p->tmp_time,p);*/
    channel[*i]=p->tmp_ch;
    time[*i]=p->tmp_time;
    (*i)++;
    treep(p->right,channel,time,i);
    free(p);
  }
}

int vdcana(struct vdcdata *vdc,struct chamber *cham){
  int i;
  switch(vdc->wire) {
  case 0:
    vdc->posid[++vdc->posid[0]]=10;break; /* if no hit */
  case 1:
    vdc->posid[++vdc->posid[0]]=3;     /*if only one hit */
    vdc->nhit[0]=1;
    vdc->nclu=1;
    break;
  default:
    vdc->nhit[0]=1;
    vdc->nclu=1;
    for(i=1;i<vdc->wire;i++){                  /* clusterize */
      if(vdc->channel[i-1]+1==vdc->channel[i])
      vdc->nhit[vdc->nclu-1]++;
      else {
	vdc->nclu++;
	vdc->nhit[vdc->nclu-1]=1;
	if(vdc->nclu==VDCNCL){
	  for(i++;i<vdc->wire;i++){
	    if(vdc->channel[i-1]+1!=vdc->channel[i]) vdc->nclu++;
	  }
	  printf("Too many clusters(%d)! (%s) b:%d e:%d\n",
		 vdc->nclu,vdc->label,block,eventcount);
	  break;
	}
      }
    }

    if(vdc->nclu > 1){                          /* if multi cluster */
      if(vdc->nclu >= 9)                        /* if more than 9 clusters */
	vdc->posid[++vdc->posid[0]]=19;
      else
	vdc->posid[++vdc->posid[0]]=vdc->nclu+10;
      return(-1);
    }
    /* if one cluster */
    vdc->posid[++vdc->posid[0]]=11;
    if(wirecheck(vdc)!=0){ /* if time-out does not occur */
      return(-2);
    }

    if(vdc->wire==2){     /* if the cluster is cosist of two wires */
#ifdef _VDC_TWOWIRE_ON
      vdc->good=1;  /* comment out when you exclude two wires event  */
#endif
      vdc->posid[++vdc->posid[0]]=5;
      vdc->dis=vdc->table[vdc->time[0]]/
	(vdc->table[vdc->time[0]]+vdc->table[vdc->time[1]]);
      if(vdc->time[0]>vdc->time[1]) vdc->min=0;
      else {
	vdc->min=1;
	vdc->dis-=1;
      }
    }
    else{
      vdc->min=go_down(vdc->time,vdc->wire,1); 
      switch(vdc->min){ /* decide drift pattern */
      case -2: /* two time peaks exist */
	vdc->posid[++vdc->posid[0]]=4;
	return(-3);
	break;
      case 0:  /* if the nearest wire is the first of the cluster */
	vdc->posid[++vdc->posid[0]]=6;
/*	vdc->good=1;*/
	break;
      default:
	if(vdc->min+1==vdc->wire){ 
	 /* if the nearest wire is the last of the cluster */
	  vdc->posid[++vdc->posid[0]]=7;
/*	  vdc->good=1;*/
	}
	else {
	  /* if the nearest wire is in the middle of the cluster */
	  vdc->posid[++vdc->posid[0]]=8;
	  vdc->good=1;
	  break;
	}
      }
    }

    if(vdc->good==1){
      /*for(i=0;i<vdc->wire;i++){
	printf("%d = %f\n",i,vdc->table[vdc->time[i]]);
	}
	printf("Min:%d\n",vdc->min);*/
      if(vdc->posid[vdc->posid[0]]==8) 
	{
#ifdef _VDC_LEASTSQRT /* do least square fit in VDC analysis */
	  vdc->dis=leastsqrt(vdc);
      /* printf("dis=%f\n\n",vdc->dis);*/
	  vdc->dis2=((vdc->table[vdc->time[(vdc->min)-1]])
		     -(vdc->table[vdc->time[(vdc->min)+1]]))
	    / ((vdc->table[vdc->time[(vdc->min)-1]])
	       +(vdc->table[vdc->time[(vdc->min)+1]]));

#else /* no _VDC_LEASTSQRT */
      /* printf("dis=%f\n\n",vdc->dis);*/
	  vdc->dis=((vdc->table[vdc->time[(vdc->min)-1]])
		     -(vdc->table[vdc->time[(vdc->min)+1]]))
	    / ((vdc->table[vdc->time[(vdc->min)-1]])
	       +(vdc->table[vdc->time[(vdc->min)+1]]));
	  vdc->dis2=leastsqrt(vdc);
#endif /* _VDC_LEASTSQRT */
	}
      vectconb(vdc->dis+(double)vdc->channel[vdc->min],
	       cham->displace,1.0,cham->position,vdc->pos);
      rotate_y(vdc->tilt_s,vdc->tilt_c,vdc->pos,vdc->rpos);
    }
  }
  /*printf("Print Error\n");
    for(i=0;i<=vdc->posid[0];i++) {
    printf("\t Error No.%d = %d\n",i,vdc->posid[i]);}
    for(i=0;i<vdc->nclu;i++){
    printf("Cluster:%d Hits:%d\n",i,vdc->nhit[i]);
    }*/
}



/* 96/09/13 
Now we assume only the event whose posid is 8 calls least sqrt.*/
  
double leastsqrt(struct vdcdata *vdc){
  double sigma[2]; /* difference between real value and linear value */
  double kai[2];
  double s[2][3],t[2][2],a[2][2];
  double y,tkai;
  int i,j,k,worst[2]={0,0};

  for(i=0;i<2;i++){
    sigma[i]=0;
    kai[i]=0;
    for(j=0;j<2;j++) {
      s[i][j]=0;
      t[i][j]=0;
      a[i][j]=0;
    }
    s[i][2]=0;
  }

  for(i=0;i<2;i++){
    double derr;
    k=0;
    while(k<vdc->min){
      derr=vdc->derr[vdc->time[k]];
      s[i][2]+=1.0/derr;
      s[i][1]+=(k-(vdc->min))*(k-(vdc->min))/derr;
      s[i][0]+=(k-(vdc->min))/derr;
      t[i][1]-=vdc->table[vdc->time[k]]*(k-(vdc->min))/derr;
      t[i][0]-=vdc->table[vdc->time[k]]/derr;
/*      printf("derr %e\n",vdc->table[vdc->time[k]]);*/
      k++;
    }
    derr=vdc->derr[vdc->time[k]];
    if(i==0) t[i][0]+=vdc->table[vdc->time[k]]/derr;
    else t[i][0]-=vdc->table[vdc->time[k]]/derr;
    s[i][2]+=1/derr;
/*    printf("derr** %e\n",vdc->table[vdc->time[k]]);*/
    k++;
    while(k<vdc->wire){
      derr=vdc->derr[vdc->time[k]];
      s[i][2]+=1/derr;
      s[i][1]+=(k-(vdc->min))*(k-(vdc->min))/derr;
      s[i][0]+=(k-(vdc->min))/derr;
      t[i][1]+=vdc->table[vdc->time[k]]*(k-vdc->min)/derr;
      t[i][0]+=vdc->table[vdc->time[k]]/derr;
/*      printf("derr %e\n",vdc->table[vdc->time[k]]);*/
      k++;
    }
/*    printf("\n");*/

    a[i][0]=(s[i][1]*t[i][0]-s[i][0]*t[i][1])
      /(s[i][2]*s[i][1]-s[i][0]*s[i][0]);
    a[i][1]=(s[i][2]*t[i][1]-s[i][0]*t[i][0])
      /(s[i][2]*s[i][1]-s[i][0]*s[i][0]);

    k=0;
    while(k < vdc->min){
      derr=vdc->derr[vdc->time[k]];
      y=-(vdc->table[vdc->time[k]])-(a[i][0]+a[i][1]*(k-(vdc->min)));
      tkai=y*y/derr;
      if(kai[i]<tkai) {
	worst[i]=k;
	kai[i]=tkai;
      }
      sigma[i]+=tkai;
      k++;
    }
    derr=vdc->derr[vdc->time[k]];
    if(i==0) y=vdc->table[vdc->time[k]]-a[i][0];
    else y=-(vdc->table[vdc->time[k]])-a[i][0];
    tkai=y*y/derr;
    if(kai[i]<tkai) {
      worst[i]=k;
      kai[i]=tkai;
    }
    sigma[i]+=tkai;
    k++;
    while(k<vdc->wire){
      derr=vdc->derr[vdc->time[k]];
      y=vdc->table[vdc->time[k]]-(a[i][0]+a[i][1]*(k-vdc->min));
      tkai=y*y/derr;
      if(kai[i]<tkai) {
	worst[i]=k;
	kai[i]=tkai;
      }
      sigma[i]+=tkai;
      k++;
    }
  }

/*
  if(vdc->time[vdc->min-1]>vdc->time[vdc->min+1]){
    if(sigma[0]<sigma[1]) vdc->dis2=0.5;
    else vdc->dis2=-0.5;
  }
  else {
    if(sigma[0]>sigma[1]) vdc->dis2=0.5;
    else vdc->dis2=-0.5;
  }*/
  
/*  
  for(i=0;i<vdc->wire;i++){
    printf("%d = %d  %f\n",i,vdc->channel[i],vdc->table[vdc->time[i]]);
  }
  printf("y=%f + %f x\n",a[0][0],a[0][1]);
  printf("y=%f + %f x\n",a[1][0],a[1][1]);

  for(i=0;i<2;i++){
    printf("s[%d][0]:%f  s[%d][1]:%f \n",i,s[i][0],i,s[i][1]);
    printf("t[%d][0]:%f  t[%d][1]:%f \n",i,t[i][0],i,t[i][1]);
  }
  printf("+min:%f  -min:%f\n",-1*a[0][0]/a[0][1],-1*a[1][0]/a[1][1]);
  printf("+sigma:%f -sigma:%f\n\n",sigma[0],sigma[1]);*/


  if(sigma[0]<sigma[1]){
    vdc->kai=sigma[0]/k;
    vdc->worst=worst[0];
    return(-1*a[0][0]/a[0][1]);
  }
  else{
    vdc->kai=sigma[1]/k;
    vdc->worst=worst[1];
    return(-1*a[1][0]/a[1][1]);
  }
}


int wirecheck(struct vdcdata *vdc){
  int i;
  for(i=0;i<vdc->wire;i++){
    if(vdc->time[i]>vdc->time_h || vdc->time[i]<vdc->time_l) {
      vdc->posid[++vdc->posid[0]]=2;
      return(i+1);
    }
  }
  return(0);
}
