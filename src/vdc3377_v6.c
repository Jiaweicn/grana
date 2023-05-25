/* vdc3388_v6.c */
/* ver0.2a   99/06/11 modify least squre routine */
/* ver0.2b   99/06/12 add routine to exclude worst wire */
/* ver0.2c   99/11/02 remove routine to exclude worst wire */
/* ver0.2d   00/01/06 change the way to decide minimum wire when posid=5 */
/* ver0.3    00/01/21 add compile option _ELIM_GAMMA to eliminate gamma events.
                      single-hit cluster can be eliminated from analysis */
/* ver0.3a   00/01/28 add compile option _LAS_OFF to ignore LAS events. */
/*                    add compile option _GR_OFF to ignore GR events. */
/* ver0.4    00/07/03 add routine to check position  */
/*                    dependence of efficiencies.                    */
/*                    add routine 'fpsimple' to deciede position */
/*                    on focal plane by one plane */
/* ver0.4a   00/07/09 fix the bug to miscount the number of wires */
/*                     in gamma elimination modes (least sqrt)  */
/* ver0.4b   05/10/03 Add las_fv and las rv. */
/*           05/10/15 Add compile option VDC_CLUSTER_CORR to treat a two peak problem */
/*                    in the VDC drift lengths. */
/* ver0.5    07/01/17 Correct a routine to track by a single plane, */
/*                    and introduce svec[3] to store the ray vector. */
/*                    Delete a routine to determin raynag. */
/* ver0.51   08/09/24 Add a routine to treat Reserved planes */
/*                    GR_FR_R, GR_RE_R, LAS_FR_R, GR_RE_R.   */
/*                    struct vdcdata grfr, grrr, lasfr, lasrr */
/* ver0.6    09/01/17 Define a function "t2dl" which converts */
/*                    time to drift length.                   */



/************************************/
/**** RAY-ID ****/
/* 0  ... Good event */
/* 2  ... Ray can not be calculated. */
/* 3  ... Front Chambers correctly hit (|DX|<36mm). */
/* 4  ... Rear Chambers correctly hit (|DX|<36mm). */
/* 11--16 ... Bad-Chamber pattern */


/**** POS-ID ****/
/* 0  ... Good event */
/* 2  ... Out of time rage */
/* 3  ... Only 1 hit wire */
/* 4  ... Two peaks inthe time structure */
/* 5  ... Only 2 hit wires */
/* 6  ... First wire is nearest */
/* 7  ... Last wire is nearest */
/* 8  ... Middle wire is nearest */
/* 9  ... Edge wire has been elimnated. Saved event by _VDC_CLUSTER_CORR */
/* 10 ... No hit */
/* 11--15 ... N cluster(s) ID=N+10 */
/* 15 ... More than 5 clusters */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ana10.h"
#include "config.h"
#include "vectcalc.h"

extern char comname[];
extern int rp,wp;
extern unsigned short regionsize,*buff;
extern unsigned long block;
extern int eventcount;
extern struct vdcdata grfx,grfu,grrx,grru;
extern struct chamber vdc_fx,vdc_fu,vdc_rx,vdc_ru;
extern struct vdcdata lasfx,lasfu,lasrx,lasru,lasfv,lasrv;
extern double *grplane[];

extern struct vdcdata grfr,grrr,lasfr,lasrr;

int vdc_3377(){
  unsigned short chamber_id,data;
  short time,module_id=0,base_ch=0,channel;
  int mod_fr_x,mod_fr_u,mod_re_x,mod_re_u,k,i;
  int mod_fr_x_l,mod_fr_u_l,mod_re_x_l,mod_re_u_l,mod_fr_v_l,mod_re_v_l;
  int mod_fr_r,mod_re_r,mod_fr_r_l,mod_re_r_l;

  rp=wp+regionsize;
  chamber_id=0;k=0;
  mod_fr_x=0;mod_fr_u=0;mod_re_x=0;mod_re_u=0;		  
  mod_fr_x_l=0;mod_fr_u_l=0;mod_re_x_l=0;mod_re_u_l=0;		  
  mod_fr_v_l=0;mod_re_v_l=0;

  mod_fr_r=0;mod_re_r=0;mod_fr_r_l=0;mod_re_r_l=0;

  while(rp>wp){
    data=buff[++wp]; 
    if(data>>15) {  /* if 3377 header */
      module_id=(data & 0x000f);
      base_ch=module_id*32;
      chamber_id=(data & 0x81f0);
      switch(chamber_id){
#ifndef _GR_OFF
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
      case GR_FR_R:
	mod_fr_r=1;break;
      case GR_RE_R:
	mod_re_r=1;break;
#endif /* _GR_OFF */
#ifndef _LAS_OFF
      case LAS_FR_X:
	if(module_id!=0) /* for LAS_FR_X sorting */
	  mod_fr_x_l=1;
	break;
      case LAS_FR_U:
	mod_fr_u_l=1;break;
      case LAS_RE_X:
	mod_re_x_l=1;break;
      case LAS_RE_U:
	mod_re_u_l=1;break;
      case LAS_FR_V:
	mod_fr_v_l=1;break;
      case LAS_RE_V:
	mod_re_v_l=1;break;
      case LAS_FR_R:
	mod_fr_r_l=1;break;
      case LAS_RE_R:
	mod_re_r_l=1;break;
#endif /* _LAS_OFF */
      default:
	/*	printf("%s:b %d:e %d:Inconsistent chamber_id(VDC3377)! %x\n",
		comname,block,eventcount,chamber_id);*/
	chamber_id=0xffff; /* indicate wrong chamber_id */
	wp=rp;
	break;
      }
    }

    else {          /* unless 3377 header */
      channel=(data>>10)+base_ch;
      time=(data & 0x03ff);
      if(time==0) continue ;
      /*      printf("%02x:\tVDC channel:%d\tchannel:%d\tbase_ch:%d\ttime:%d\n",
	      chamber_id,channel,data>>10,base_ch,time);*/

      /** for E290 Nov. 21 2006 ***************/
#if 0
      if(((module_id+chamber_id)==0x8188)&&(data>>10)>15) chamber_id=LAS_RE_X;
      if(((module_id+chamber_id)==0x81c8)&&(data>>10)>15)  channel-=16;
#endif
      /** for E290 Nov. 21 2006 ***************/

      switch(chamber_id) {
#ifndef _GR_OFF
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
      case GR_FR_R:
		vdcread(&grfr,channel,time);
		break;
      case GR_RE_R:
		vdcread(&grrr,channel,time);
		break;
#endif /* GR_OFF */
#ifndef _LAS_OFF
      case LAS_FR_X:
		if(module_id==0) mod_fr_x_l=1; /* for LAS_FR_X sorting */
		vdcread(&lasfx,channel,time);
		break;
      case LAS_FR_U:
		vdcread(&lasfu,channel,time);
		break;
      case LAS_RE_X:
	        vdcread(&lasrx,channel,time);
		break;
      case LAS_RE_U:
		vdcread(&lasru,channel,time);
		break;
      case LAS_FR_V:
		vdcread(&lasfv,channel,time);
		break;
      case LAS_RE_V:
		vdcread(&lasrv,channel,time);
		break;
      case LAS_FR_R:
		vdcread(&lasfr,channel,time);
		break;
      case LAS_RE_R:
		vdcread(&lasrr,channel,time);
		break;
#endif /* LAS_OFF */
      case 0xffff:
		break;
      default:
		printf("%s:No 3377 header word exists.(%4x) b:%ld e:%d\n"
			   ,comname,chamber_id,block,eventcount);
		wp=rp;
		return(-1);
		break;
      }
    }
  }

  /*******sorting**************/
#ifndef _GR_OFF
  if(mod_fr_x==1) vdcsort(&grfx);
  if(mod_fr_u==1) vdcsort(&grfu);
  if(mod_re_x==1) vdcsort(&grrx);
  if(mod_re_u==1) vdcsort(&grru);
  if(mod_fr_r==1) vdcsort(&grfr);
  if(mod_re_r==1) vdcsort(&grrr);
#endif /* GR_OFF */
#ifndef _LAS_OFF
  if(mod_fr_x_l==1) vdcsort(&lasfx);
  if(mod_fr_u_l==1) vdcsort(&lasfu);
  if(mod_re_x_l==1) vdcsort(&lasrx);
  if(mod_re_u_l==1) vdcsort(&lasru);
  if(mod_fr_v_l==1) vdcsort(&lasfv);
  if(mod_re_v_l==1) vdcsort(&lasrv);
  if(mod_fr_r_l==1) vdcsort(&lasfr);
  if(mod_re_r_l==1) vdcsort(&lasrr);
#endif /* LAS_OFF */
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
    printf("Event buffer over flow!(%s) %d (%ld:%d)\n",
	   vdc->label,vdc->wire,block,eventcount);
}

void vdcsort(struct vdcdata *vdc){
  int i;
  sort3377(vdc->channel,vdc->time,vdc->wire,vdc->label);
/*  for(i=0;i<vdc->wire;i++){
      printf("%s\t channel:%d\t time:%d\n",vdc->label,
    vdc->channel[i],vdc->time[i]);
    }*/
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
    vdc->posid|=0x0400; /* No hit (10) */
    return(-1);
    break; /* if no hit */
  case 1:
    vdc->posid|=0x0008; /* only one hit (3) */
    vdc->nhit[0]=1;
    vdc->nclu=1;
    return(-1);
    break;
  default:
    vdc->nhit[0]=1;
    vdc->nclu=1;
    vdc->cclu[0]=0;
    for(i=1;i<vdc->wire;i++){                  /* clusterize */
      if(vdc->channel[i-1]+1==vdc->channel[i])
	vdc->nhit[vdc->nclu-1]++;
      else {
	vdc->nhit[vdc->nclu]=1;
	vdc->cclu[vdc->nclu]=i;
	vdc->nclu++;
	if(vdc->nclu==VDCNCL){
	  for(i++;i<vdc->wire;i++){
	    if(vdc->channel[i-1]+1!=vdc->channel[i]) vdc->nclu++;
	  }
	  printf("Too many clusters(%d)! (%s) b:%ld e:%d\n",
		 vdc->nclu,vdc->label,block,eventcount);
	  break;
	}
      }
    }

    for(i=0;i<vdc->nclu;i++){
      if(vdc->nhit[i]>1){
		vdc->nlhit[vdc->nlclu]=vdc->nhit[i];
		vdc->clclu[vdc->nlclu++]=vdc->cclu[i];
      }
    }


#ifdef _ELIM_GAMMA
/* calc position here */
  vdc_calcpoint(vdc,cham,vdc->nlclu,vdc->nlhit,vdc->clclu);
#else
  vdc_calcpoint(vdc,cham,vdc->nclu,vdc->nhit,vdc->cclu);
#endif
  }

  return(0);
}



/* 96/09/13 
Now we assume only the event whose posid is 8 calls least sqrt.*/
  
double leastsqrt(struct vdcdata *vdc,int nhit,int cclu){
  double sigma[2]; /* difference between real value and linear value */
  double kai[2];
  double s[2][3],t[2][2],a[2][2];
  double y,tkai;
  double resi[2][VDCEVE_L];
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

  nhit+=cclu;
  for(i=0;i<2;i++){
    double derr;
    k=cclu;
    while(k<vdc->min){
      derr=vdc->derr[vdc->time[k]];
      s[i][2]+=1.0/derr;
      s[i][1]+=(k-(vdc->min))*(k-(vdc->min))/derr;
      s[i][0]+=(k-(vdc->min))/derr;
#ifndef _VDC_INTERPOLATION
      t[i][1]-=vdc->table[vdc->time[k]]*(k-(vdc->min))/derr;
      t[i][0]-=vdc->table[vdc->time[k]]/derr;
#else
      t[i][1]-=t2dl(vdc,k,1)*(k-(vdc->min))/derr;
      t[i][0]-=t2dl(vdc,k,1)/derr;
#endif
/*      printf("derr %e\n",vdc->table[vdc->time[k]]);*/
      k++;
    }
    derr=vdc->derr[vdc->time[k]];
#ifndef _VDC_INTERPOLATION
    if(i==0) t[i][0]+=vdc->table[vdc->time[k]]/derr;
    else t[i][0]-=vdc->table[vdc->time[k]]/derr;
#else
    if(i==0) t[i][0]+=t2dl(vdc,k,1)/derr;
    else t[i][0]-=t2dl(vdc,k,1)/derr;
#endif
    s[i][2]+=1/derr;
/*    printf("derr** %e\n",vdc->table[vdc->time[k]]);*/
    k++;
    while(k<nhit){
      derr=vdc->derr[vdc->time[k]];
      s[i][2]+=1/derr;
      s[i][1]+=(k-(vdc->min))*(k-(vdc->min))/derr;
      s[i][0]+=(k-(vdc->min))/derr;
#ifndef _VDC_INTERPOLATION
      t[i][1]+=vdc->table[vdc->time[k]]*(k-vdc->min)/derr;
      t[i][0]+=vdc->table[vdc->time[k]]/derr;
#else
      t[i][1]+=t2dl(vdc,k,1)*(k-vdc->min)/derr;
      t[i][0]+=t2dl(vdc,k,1)/derr;
#endif
/*      printf("derr %e\n",vdc->table[vdc->time[k]]);*/
      k++;
    }
/*    printf("\n");*/

    a[i][0]=(s[i][1]*t[i][0]-s[i][0]*t[i][1])
      /(s[i][2]*s[i][1]-s[i][0]*s[i][0]);
   a[i][1]=(s[i][2]*t[i][1]-s[i][0]*t[i][0])
      /(s[i][2]*s[i][1]-s[i][0]*s[i][0]);

    k=cclu;
    while(k < vdc->min){
      derr=vdc->derr[vdc->time[k]];
#ifndef _VDC_INTERPOLATION
      y=-(vdc->table[vdc->time[k]])-(a[i][0]+a[i][1]*(k-(vdc->min)));
#else
      y=-(t2dl(vdc,k,1))-(a[i][0]+a[i][1]*(k-(vdc->min)));
#endif
      resi[i][k]=y;
      tkai=y*y/derr/derr;
      if(kai[i]<tkai) {
	worst[i]=k;
	kai[i]=tkai;
      }
      sigma[i]+=tkai;
      k++;
    }
    derr=vdc->derr[vdc->time[k]];
#ifndef _VDC_INTERPOLATION
    if(i==0) y=vdc->table[vdc->time[k]]-a[i][0];
    else y=-(vdc->table[vdc->time[k]])-a[i][0];
#else
    if(i==0) y=t2dl(vdc,k,1)-a[i][0];
    else y=-(t2dl(vdc,k,1))-a[i][0];
#endif
    resi[i][k]=y;
    tkai=y*y/derr/derr;
    if(kai[i]<tkai) {
      worst[i]=k;
      kai[i]=tkai;
    }
    sigma[i]+=tkai;
    k++;
    while(k<nhit){
      derr=vdc->derr[vdc->time[k]];
#ifndef _VDC_INTERPOLATION
      y=vdc->table[vdc->time[k]]-(a[i][0]+a[i][1]*(k-vdc->min));
#else
      y=t2dl(vdc,k,1)-(a[i][0]+a[i][1]*(k-vdc->min));
#endif
      resi[i][k]=y;
      tkai=y*y/derr/derr;
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
  

/* Output tracking information */
#if 0
  /*
  if((sigma[0]<sigma[1]&&fabs(-1*a[0][0]/a[0][1])<0.06) ||
  (sigma[1]<sigma[0]&&fabs(-1*a[1][0]/a[1][1])<0.06)){*/
  if(vdc->wire>=4){
    printf("%10.5f\n",(double)vdc->wire);
    for(i=0;i<vdc->min;i++){
      printf("%10.5f %10.5f %10.5f %10.5f %10.5f\n",
	     (double)i,(double)vdc->channel[i]-vdc->channel[vdc->min]
	     ,-t2dl(vdc,i,1),-t2dl(vdc,i,1),vdc->derr[vdc->time[i]]);
    }
    printf("%10.5f %10.5f %10.5f %10.5f %10.5f\n",
	   (double)i,(double)vdc->channel[i]-vdc->channel[vdc->min]
	   ,t2dl(vdc,i,1),-t2dl(vdc,i,1),vdc->derr[vdc->time[i]]);
    i++;
    for(;i<vdc->wire;i++){
      printf("%10.5f %10.5f %10.5f %10.5f %10.5f\n",
	     (double)i,(double)vdc->channel[i]-vdc->channel[vdc->min]
	     ,t2dl(vdc,i,1),t2dl(vdc,i,1),vdc->derr[vdc->time[i]]);
    }
    printf("%10.5f %10.5f %10.5f %10.5f\n",
	   a[0][0],a[0][1],sigma[0],-1*a[0][0]/a[0][1]);
    printf("%10.5f %10.5f %10.5f %10.5f\n",
	   a[1][0],a[1][1],sigma[1],-1*a[1][0]/a[1][1]);
    /*  printf("#### %s\n",vdc->label);*/
  }
#endif

/*
  printf("y=%f + %f x\n",a[0][0],a[0][1]);
  printf("y=%f + %f x\n",a[1][0],a[1][1]);*/

/*  
  for(i=0;i<2;i++){
    printf("s[%d][0]:%f  s[%d][1]:%f \n",i,s[i][0],i,s[i][1]);
    printf("t[%d][0]:%f  t[%d][1]:%f \n",i,t[i][0],i,t[i][1]);
  }
  printf("+min:%f  -min:%f\n",-1*a[0][0]/a[0][1],-1*a[1][0]/a[1][1]);
  printf("+sigma:%f -sigma:%f\n\n",sigma[0],sigma[1]);*/


  if(sigma[0]<sigma[1]){
    vdc->kai=sigma[0]/k;
    vdc->worst=worst[0];
    for(i=cclu;i<cclu+nhit;i++) vdc->residue[i]=resi[0][i];
    return(-1*a[0][0]/a[0][1]);
  }
  else{
    vdc->kai=sigma[1]/k;
    vdc->worst=worst[1];
    for(i=cclu;i<cclu+nhit;i++) vdc->residue[i]=resi[1][i];
    return(-1*a[1][0]/a[1][1]);
  }
}


int wirecheck(struct vdcdata *vdc,int nhit,int fwire){
  int i,j;
  j=nhit+fwire;
  for(i=fwire;i<j;i++){
    if(vdc->time[i]>vdc->time_h || vdc->time[i]<vdc->time_l) {
      vdc->posid|=0x0004; /* Out of range (2) */
/*      fprintf(stderr,"low;%d  high;%d\n",vdc->time_l,vdc->time_h);
      fprintf(stderr,"%s;%d\n",vdc->label,vdc->time[i]);*/
      return(i+1);
    }
  }
  return(0);
}


int vdc_calcpoint(struct vdcdata *vdc,struct chamber *cham,
		     int nclu,int *hit,int *cclu){
  int fwire; /* first wire in the unique cluster */
  int nhit;   /* number of hit in the unique cluster */
  int i; /* tmp  */
  fwire=*cclu;
  nhit=*hit;

  if(nclu >= 5) vdc->posid|=0x8000; /* More than 5 clusters (15) */
  else vdc->posid|=1<<(nclu+10);
  if(nclu > 1 || nclu==0) return(-1);   /* if multi cluster or no cluster */

  /* if one cluster */
  if(wirecheck(vdc,nhit,fwire)!=0){ /* if time-out does not occur */
    return(-2);
  }

  if(nhit==2){     /* if the cluster is cosist of two wires */
#ifdef _VDC_TWOWIRE_ON
    vdc->good=1;  /* comment out when you exclude two wires event  */
#if 0    /* Determination of Ray-vector by a single plane */
    /* 10.0 mean GRVDC drift space */
    vdc->svec[0]=cham->displace[0];
    vdc->svec[1]=0;
    vdc->svec[2]=(vdc->table[vdc->time[fwire]]+
		  vdc->table[vdc->time[fwire+1]])*10.0;
#endif
#endif
    vdc->posid|=0x0020; /* Two wires (5) */

#ifndef _VDC_INTERPOLATION
    vdc->dis=vdc->table[vdc->time[fwire]]/
      (vdc->table[vdc->time[fwire]]+vdc->table[vdc->time[fwire+1]]);
#else
    vdc->dis=t2dl(vdc,fwire,1)/
      (t2dl(vdc,fwire,1)+t2dl(vdc,fwire+1,1));
#endif
    if(vdc->time[fwire]>vdc->time[fwire+1]) vdc->min=fwire;
    else {
      vdc->min=fwire+1;
      vdc->dis-=1;
    }
  }
  else{
    vdc->min=go_down(vdc->time,nhit,fwire+1); 
    /* decide drift pattern */
    if(vdc->min==-2){ /* two time peaks exist */
      vdc->posid|=0x0010; /* Two peaks (4) */
#ifdef _VDC_CLUSTER_CORR
      /***************************************************/
      if(nhit>3){
	/* if the first wire is eliminated .... */
	vdc->min=go_down(vdc->time,nhit-1,fwire+2);
	if(vdc->min!=-2&&vdc->min!=(fwire+1)&&vdc->min!=(fwire+nhit-1)){
	  vdc->posid|=0x0200; /* Edge wire has been elimated. (9) */
	  fwire++;
	  nhit--;
	}
	else{
	  /* if the last wire is eliminated .... */
	  vdc->min=go_down(vdc->time,nhit-1,fwire+1);
	  if(vdc->min!=-2&&vdc->min!=fwire&&vdc->min!=(fwire+nhit-2)){
	    vdc->posid|=0x0200; /* Edge wire has been elimated. (9) */
	    nhit--;
	  }
	  else{
	    return(-3);
	  }
	  hit[0]=nhit;
	  cclu[0]=fwire;
	}
      }
      /***************************************************/	  
#else
      return(-3);
#endif
    }
    if(vdc->min==fwire){/* if the nearest wire is the first of the cluster */
      vdc->posid|=0x0040; /* The first wire is the nearest. (6) */
      return(-4);
    }
    else {
      if(vdc->min==(fwire+nhit-1)){
	/* if the nearest wire is the last of the cluster */
	vdc->posid|=0x0080; /* The last wire is the nearest. (7) */
	return(-5);
      }
      else {
	/* if the nearest wire is in the middle of the cluster */
	vdc->posid|=0x0100; /* The nearest wire is middle. (8) */
	/* 10.0 mean GRVDC drift space */

#ifndef _VDC_INTERPOLATION
	vdc->res[0]=vdc->table[vdc->time[vdc->min]];
	vdc->res[1]=(vdc->table[vdc->time[(vdc->min-1)]]
		     -vdc->table[vdc->time[(vdc->min+1)]])/2.0;
#else
	vdc->res[0]=t2dl(vdc,vdc->min,1);
	vdc->res[1]=(t2dl(vdc,vdc->min-1,1)-t2dl(vdc,vdc->min+1,1))/2.0;
#endif

#if 0    /* Determination of Ray-vector by a single plane */
	vdc->svec[0]=cham->displace[0]*2.0;
	vdc->svec[1]=0;
	vdc->svec[2]=(vdc->table[vdc->time[(vdc->min)-1]]
		      +vdc->table[vdc->time[(vdc->min)+1]])*10;
	vdc->fppos=atan(vdc->svec[0]/vdc->svec[2])*R_TO_D+VDC_TILT;
#endif
	vdc->good=1;
      }
    }
  }

  
  if(vdc->good==1){
    vdc->posid|=0x0001; /* Good Event (0) */
    if(vdc->posid&0x0100){
#ifdef _VDC_LEASTSQRT /* do least square fit in VDC analysis */
      vdc->dis=leastsqrt(vdc,nhit,fwire);
      /* printf("dis=%f\n\n",vdc->dis);*/
      vdc->dis2=((vdc->table[vdc->time[(vdc->min)-1]])
		 -(vdc->table[vdc->time[(vdc->min)+1]]))
	/ ((vdc->table[vdc->time[(vdc->min)-1]])
	   +(vdc->table[vdc->time[(vdc->min)+1]]));
#else /* no _VDC_LEASTSQRT */
      /* printf("dis=%f\n\n",vdc->dis);*/
#ifndef _VDC_INTERPOLATION
      vdc->dis=((vdc->table[vdc->time[(vdc->min)-1]])
		-(vdc->table[vdc->time[(vdc->min)+1]]))
	/ ((vdc->table[vdc->time[(vdc->min)-1]])
	   +(vdc->table[vdc->time[(vdc->min)+1]]));
#else
      vdc->dis=(t2dl(vdc,vdc->min-1,1)-t2dl(vdc,vdc->min+1,1))
	/ (t2dl(vdc,vdc->min-1,1)+t2dl(vdc,vdc->min+1,1));
#endif
      vdc->dis2=leastsqrt(vdc,nhit,fwire);
#endif /* _VDC_LEASTSQRT */

    }
    vectconb(vdc->dis+(double)vdc->channel[vdc->min],
	     cham->displace,1.0,cham->position,vdc->pos);
    rotate_y(vdc->tilt_s,vdc->tilt_c,vdc->pos,vdc->rpos);
#if 0    /* Determination of Ray-vector by a single plane */
    rotate_y(vdc->tilt_s,vdc->tilt_c,vdc->svec,vdc->svec);
    plane_eq(vdc->svec,cham->wiredirec,vdc->rpos,vdc->plane);
#endif
    return(0);
  }
  else return(-1);
}

void bookvdc(struct vdcdata *vdc, int histid){
  int k,j=19;
  for(k=0;k<vdc->wire;k++) {  
    book1(histid,(float)vdc->channel[k]);
    book1(histid+4,(float)vdc->time[k]);
  }

  j=19;
  for(k=0;k<vdc->nclu;k++){
    if(vdc->nhit[k]>19) book1(histid+8,(float)j);
    else book1(histid+8,(float)vdc->nhit[k]);
  }

  j=19;
  if(vdc->nlclu>19) book1(histid+12,(float)j);
  else book1(histid+12,(float)vdc->nlclu);

  if(vdc->good==1) {j=0;book1(histid+16,(float)j);}
  for(k=0;k<16;k++){
    if(vdc->posid&(1<<k)) book1(histid+16,(float)k);
  }

  for(k=0;k<vdc->wire;k++) {
    book1d(histid+20,vdc->table[vdc->time[k]]);
  }

  if(vdc->good==1){
    book1d(histid+24,vdc->pos[0]);
    book1d(histid+28,vdc->dis);
  }
}  


double simplefp(struct vdcdata *vdc,struct chamber *cham){
  static double xzplane[4]={0,1,0,0};
  double rayplane[4];
  double fppos[3];

  rayplane[0]=-1.0;
  rayplane[1]=0;
  rayplane[2]=tan(vdc->rayang*D_TO_R);
  rayplane[3]=-vdc->rpos[0]+rayplane[2]*vdc->rpos[2];
  
  calcpoint(xzplane,rayplane,grplane[9],fppos);
  return(fppos[0]);
}

int mwdcana(struct vdcdata *mwdc){
  int i,j,iwire=0;
  int pwire,cwire;

  for(i=0;i<mwdc->wire;i++) mwdc->nhit[mwdc->channel[i]/16]++;
  for(i=0;i<4;i++){
    pwire=-2;
    for(j=0;j<mwdc->nhit[i];j++){
      cwire=mwdc->channel[iwire++]%16;
      if(cwire!=(pwire+1)){
	/* if the next wire is not hit */
	mwdc->cclu[i]++;
	mwdc->nlhit[i]=1;
	mwdc->plane[i]=(double)cwire;
      }
      else {
	/* if the next wire is not hit */
	mwdc->nlhit[i]++;
	mwdc->plane[i]+=(double)cwire;
      }
      pwire=cwire;
    }
    if(mwdc->cclu[i]==1){
      mwdc->plane[i]/=(double)mwdc->nlhit[i];
      mwdc->good|=(1<<i);
    }
  }
  return(0);
}

int mwdceff(struct vdcdata *mwdc){
  if((mwdc->good&0xe)==0xe) mwdc->clclu[0]++;
  if((mwdc->good&0xd)==0xd) mwdc->clclu[1]++;
  if((mwdc->good&0xb)==0xb) mwdc->clclu[2]++;
  if((mwdc->good&0x7)==0x7) mwdc->clclu[3]++;
  if(mwdc->good==0xf) mwdc->eff++;
  return(0);
}

double t2dl(struct vdcdata *vdc, int iwire, int iflag){
  /* iflag  0 ... Use descrete value.    */
  /*        1 ... Linear interpolation with a random number */
  /*        2 ... 3rd order interpolation with a random number */
  double dlength;

  dlength=vdc->table[vdc->time[iwire]];
#if 0
  printf("hoghoge:%3d  %5d  %10.5f   ",iwire,vdc->time[iwire],
	 dlength);
#endif
  if(vdc->time[iwire]<=vdc->time_h&&vdc->time[iwire]>=vdc->time_l){
    double tmpa,tmpb;
    switch(iflag){
    case 0:
      break;
    case 1:
      tmpb=drand48();tmpa=1.0-tmpb;
      dlength=vdc->table[vdc->time[iwire]]*tmpa
	+vdc->table[vdc->time[iwire]+1]*tmpb;
      break;
    case 2:
      tmpb=drand48();tmpa=1.0-tmpb;
      dlength=vdc->table[vdc->time[iwire]]*tmpa
	+vdc->table[vdc->time[iwire]+1]*tmpb
	+(vdc->derr[vdc->time[iwire]]*tmpa*(tmpa*tmpa-1.0)
	  +vdc->derr[vdc->time[iwire]+1]*tmpb*(tmpb*tmpb-1.0))/6.0;
      break;
    default:
      fprintf(stderr,"%s:t2dl:Undefined iflag(%d).\n",comname,iflag);
      exit(-1);
    }
  }
#if 0
  printf(":%10.5f   \n",dlength);
#endif
  return(dlength);
}

