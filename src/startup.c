/* for ana_v0.71
       startup routine 引数の処理をおこなう。*/
/*  00/01/24 block size に k(=1024) を許す  */
/*  03/07/10 いろいろ追加  */
/*           option -rp */
/*           fopenerror, check_parm, check_targ, readpar */
/*           kinema.h をインクルード */
/*  05/07/18 global section の名前を引数から定義できるようにした。*/
/*  05/11/29 rp のオペランドに Y_TGT, DEX, Z3 を追加 */
/*  05/11/30 rp のオペランドに RHO を追加 */
/*  07/02/01 rp のオペランドに GR_MODE を追加 */
/*  07/02/06 rp のオペランドに GR_RFCW を追加 */
/*  07/02/06 rp のオペランドに GR_YP を追加 */
/*  07/02/06 rp のオペランドに GR_YB を追加 */
/*  09/04/02 rp のオペランドに GR_PID1 を追加 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "router.h"
#include "kinema.h"
#include "ana10.h"
#include "config.h"

#define BLD 1

extern unsigned short *buff,*rawbuff;
extern size_t SIZEOFBLK,WORDOFBLK;
extern struct runparm grcr;
extern char gname[L_3377];


void argerror(){
  printf
  ("Usage: ana [-vdc] [-help] [-global name] [-online hostname] [-port port] [bldfile] [hbfile]\n");
  exit(-1);
}

void drifttime(){}

int check_option(char *arg){
  const struct {char *option; int i;} 
  table[]={{"-vdc",1},{"-help",2},{"-",3},{"-global",4},{"-online",5},
		   {"-port",6},{"-b",7},{"-rp",8}};
  int j;
  for(j=0;j<8;j++){ 
	/* when you add new option, you MUST change an above line.*/
    if(strcmp(arg,table[j].option)==0)
      return(table[j].i);
  }
  return(-1);
}

int check_parm(char *arg,int *nope){
  /* Return Value ....
     > 0 : Operand ID
      -1 : Unknown operand */
  const struct {char *parm; int i; int nope;}
  table[]={{"M1",1,1},{"M2",2,1},{"M3",3,1},{"M4",4,1},
	   {"E_LAB",5,1},{"GRANG",6,1},{"B_GR",7,1},{"DPDXGR",8,1},
	   {"Y_TGT",9,1},{"DEX",10,2},{"Z3",11,1},{"RHO",12,1},
	   {"GR_MODE",13,1},{"GR_RFCW",14,3},
	   {"GR_YP",15,2},{"GR_YB",16,4},{"GR_PID1",17,2}

  };
  int j;

  for(j=0;j<17;j++){
	/* when you add new operand, you MUST change an above line.*/
	if(strcmp(arg,table[j].parm)==0){
	  *nope=table[j].nope;
	  return(table[j].i);
	}
  }
  return(-1);
}

double check_targ(char *targ){
  int j,k,len,loop;
  double tmass=0;
  struct {
	char index[5];
	double a;
	double d;
  } toi[]={
	{"p",    1,  6.778},
	{"n",    1,  8.071},
	{"d",    2, 12.624},
	{"t",    3, 14.439},
	{"3he",  3, 13.909},
	{"a",    4,  2.425},
	{"11c", 11, 10.650},
	{"11b", 11,  8.668},
	{"12c", 12,  0.000},
	{"13c", 13,  3.125},
	{"16o", 16, -4.737},
	{"24mg", 24, -13.933},
	{"28si", 28, -21.492},
	{"40ca", 40, -38.546},
	{"58ni", 58, -60.223},
	{"197au", 197, -31.1410},
	{"208pb", 208, -21.748},
  };
  int ntar=17;
  /* when you add new target, you MUST change ntar.*/
  
  k=strlen(targ);
  len=(k>1?2:1);
  for(loop=0;loop<k;loop++){
	char *tmpchar;
	tmpchar=targ+loop;
	if(*tmpchar>0x40 && *tmpchar<0x5b) *tmpchar+=0x20;
  }
  for(j=0;j<ntar;j++){
	if(strcmp(targ,toi[j].index)==0) {
	  tmass=toi[j].a*AMU+toi[j].d;
	  break;
	}
  }
  if(j==ntar){
	k=sscanf(targ,"%lf",&tmass);
	if(k!=1) tmass=0;
  }
  return(tmass);
}

int readpar(char *rpfile,struct runparm *parm) {
  int nline=0,iword,nope=0,iparm;
  FILE *rpfd;
  char cpl[400];
  char tmpword[5][100];
  
  rpfd=fopen(rpfile,"r");
  if(rpfd==NULL) fopenerror(rpfile);
  while(fgets(cpl,400,rpfd)!=NULL){
    nline++;
    if(cpl[0]=='#') continue;
    iword=sscanf(cpl,"%s %s %s %s %s",
		 tmpword[0],tmpword[1],tmpword[2],tmpword[3],tmpword[4]);
    if(iword==0) {
      printf("%s:line %d: Blank line is prohibitted.\n",rpfile,nline);
      fclose(rpfd);
      exit(-1);
    }
    iparm=check_parm(tmpword[0],&nope);
    
    if(iword!=(nope+1)){
      printf("%s:line %d: %s requires %d operand(s) "
	     "(You specified %d operand(s)).\n"
	     ,rpfile,nline,tmpword[0],nope,iword-1);
      fclose(rpfd);
      exit(-1);
    }
    switch(iparm){
    case 1:  /* M1 */
      parm->m1=check_targ(tmpword[1]);
      break;
    case 2:  /* M2 */
      parm->m2=check_targ(tmpword[1]);
      break;
    case 3:  /* M3 */
      parm->m3=check_targ(tmpword[1]);
      break;
    case 4:  /* M4 */
      parm->m4=check_targ(tmpword[1]);
      break;
    case 5: /* E_LAB */
      if(sscanf(tmpword[1],"%lf",&parm->K1)!=1) {
	printf("%s:line %d: Illeagal E_LAB.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      if(parm->K1<=0){
	printf("%s:line %d: E_LAB must be positive.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 6: /* GRANG */
      if(sscanf(tmpword[1],"%lf",&parm->grang)!=1) {
	printf("%s:line %d: Illeagal GRANG.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 7: /* B_GR */
      if(sscanf(tmpword[1],"%lf",&parm->bgr)!=1) {
	printf("%s:line %d: Illeagal B_GR.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 8: /* DPDXGR */
      if(sscanf(tmpword[1],"%lf",&parm->dpdx)!=1) {
	printf("%s:line %d: Illeagal DPDXGR.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
	  }
      break;
    case 9: /* Y_TGT */
      if(sscanf(tmpword[1],"%lf",&parm->ytgt)!=1) {
	printf("%s:line %d: Illeagal Y_TGT.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 10: /* DEX */
      if((sscanf(tmpword[1],"%lf",&parm->dex[0])!=1)||
	 (sscanf(tmpword[2],"%lf",&parm->dex[1])!=1)) {
	printf("%s:line %d: Illeagal Y_TGT.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 11: /* Z3 */
      if(sscanf(tmpword[1],"%lf",&parm->z3)!=1) {
	printf("%s:line %d: Illeagal Z3.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 12: /* RHO */
      if(sscanf(tmpword[1],"%lf",&parm->rho)!=1) {
	printf("%s:line %d: Illeagal RHO.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 13: /* GR_MODE */
      if(strlen(tmpword[1])>8){
	printf("%s:line %d:Too long operand fro GR_MODE\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      if(sscanf(tmpword[1],"%s",parm->grmode)!=1) {
	printf("%s:line %d: Illeagal RHO.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      else{
#define N_GRMODE 2	    
	char *grmode[N_GRMODE]={"NORM","UNF"};
	int itmp;
	for(itmp=0;itmp<N_GRMODE;itmp++){
	  if(strcmp(parm->grmode,grmode[itmp])==0) {
	    parm->igrmode=itmp;
	    break;
	  }	    
	}
	if(itmp==N_GRMODE){
	  printf("%s:line %d:Undefined GRMODE %s.\n",
		 rpfile,nline,parm->grmode);
	  fclose(rpfd);
	  exit(-1);
	}
	printf("##%s##\n",parm->grmode);
      }
      break;
    case 14: /* GR_RFCW */
      if((sscanf(tmpword[1],"%lf",&parm->rfc[0])!=1)||
	 (sscanf(tmpword[2],"%lf",&parm->rfc[1])!=1)||
	 (sscanf(tmpword[3],"%lf",&parm->rfw)!=1)
	 ) {
	printf("%s:line %d: Illeagal GR_RFCW.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 15: /* GR_YP */
      if((sscanf(tmpword[1],"%lf",&parm->yp[0])!=1)||
	 (sscanf(tmpword[2],"%lf",&parm->yp[1])!=1)) {
	printf("%s:line %d: Illeagal GR_YP.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 16: /* GR_YB */
      if((sscanf(tmpword[1],"%lf",&parm->yb[0])!=1)||
	 (sscanf(tmpword[2],"%lf",&parm->yb[1])!=1)||
	 (sscanf(tmpword[3],"%lf",&parm->yb[2])!=1)||
	 (sscanf(tmpword[4],"%lf",&parm->yb[3])!=1)) {
	printf("%s:line %d: Illeagal GR_YB.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    case 17: /* GR_PID1 */
      if((sscanf(tmpword[1],"%lf",&parm->grpid1[0])!=1)||
	 (sscanf(tmpword[2],"%lf",&parm->grpid1[1])!=1)) {
	printf("%s:line %d: Illeagal GR_PID1.\n",rpfile,nline);
	fclose(rpfd);
	exit(-1);
      }
      break;
    }
  }
  parm->brhogr=parm->bgr*parm->rho*CV/1.0e9*parm->z3;
  fclose(rpfd);
  writepar(stdout,parm);
  return(0);
}


int writepar(FILE *fd,struct runparm *parm)
{
  fprintf(fd,"m1=%8.2f  m2=%8.2f  m3=%8.2f (z3=%4.1f)  m4=%8.2f\n",
		 parm->m1,parm->m2,parm->m3,parm->z3,parm->m4);
  fprintf(fd,"K1=%5.1f  GRANG=%5.1f  BGR=%8.3f   RHO=%7.5f",
		 parm->K1,parm->grang,parm->bgr,parm->rho);
  fprintf(fd,"   dp/dx=%7.4e\n",parm->dpdx);
  fprintf(fd,"Y_TGT=%6.2f mm  DEX[0]=%6.2f  DEX[1]=%6.2f\n",
		 parm->ytgt,parm->dex[0],parm->dex[1]);
  fprintf(fd,"GR_RFC1=%6.2f   GR_RFC2=%6.2f  GR_RFW=%6.2f\n",
		 parm->rfc[0],parm->rfc[1],parm->rfw);
  fprintf(fd,"GR_YP_L=%6.2f   GR_YP_H=%6.2f\n",
		 parm->yp[0],parm->yp[1]);
  fprintf(fd,"GR_YB1_L=%6.2f  GR_YB1_H=%6.2f"
	  "  GR_YB2_L=%6.2f  GR_YB2_H=%6.2f\n",
		 parm->yb[0],parm->yb[1],parm->yb[2],parm->yb[3]);
  fprintf(fd,"GR_MODE=%s\n",parm->grmode);
  fprintf(fd,"GR_PID1_L=%6.2f   GR_PID1_H=%6.2f\n",
		 parm->grpid1[0],parm->grpid1[1]);
  return(0);
}


	  

int startup(int argc,char *argv[],int *fd,char *hbfile) {
  int i,j,k,flag,port=0;
  int buffsize;
  int lgname;
  char *filename[2];
  char hostname[80];
  flag=0;j=0;
  filename[0]=NULL;filename[1]=NULL;

  SIZEOFBLK=BUFF_SIZE;    /* set default block size */

  /**** Set Default Value ****/
  grcr.m1=MP;grcr.m2=12*AMU;grcr.m3=grcr.m1;grcr.m4=grcr.m2;
  grcr.K1=392;grcr.grang=10;grcr.bgr=952;grcr.dpdx=-6.480806e-5;
  grcr.ytgt=0;grcr.dex[0]=0;grcr.dex[1]=1.0;
  grcr.z3=1.0;grcr.rho=3.0;
  grcr.brhogr=grcr.bgr*grcr.rho*CV*1e-9;
  strcpy(grcr.grmode,"NORM");grcr.igrmode=1;
  strcpy(gname,"TEST");
  grcr.rfc[0]=RFC1;grcr.rfc[1]=RFC2;grcr.rfw=RFCW;
  grcr.yp[0]=Y_C_PEAK_L;grcr.yp[1]=Y_C_PEAK_H;
  grcr.yb[0]=Y_C_BACK1_L;grcr.yb[1]=Y_C_BACK1_H;
  grcr.yb[2]=Y_C_BACK2_L;grcr.yb[3]=Y_C_BACK2_H;
  grcr.grpid1[0]=P_ID_L;grcr.grpid1[1]=P_ID_H;
				      
  
  for(i=1;i<argc;i++){
    if(*(argv[i])=='-'){
      switch(check_option(argv[i])){
      case 1: /* vdc calibration */
	flag|=0x1;
	printf("Option -vdc is obsoleted.\n");
	exit(-1);
	break;
      case 2: /* help */
	printf("Usage: ana [-vdc] [-help] [-global name] [-online hostname] [-port port] [bldfile] [hbfile] [-b blocksize] [-rp parameter file]\n");
	printf("-vdc: Ana makes VDC time-position table (obsolete).\n");
	printf("-help: Display this help.\n");
	printf("-global [name]: Use global section. \n");
	printf("            [name] is not case sensitive.\n");
	printf("            If the last 4 characters of [name] are \".bld\",\n");
	printf("            ana will ignore it and use default name.\n");
        printf("            The default name is \"TEST\".\n");
	printf("-online: Online analysis. When this option is specified,\n");
	printf("          ana enables \"-global\" option automatically.\n");
	printf("         hostname must be specified following this option.\n");
	printf("-port: Specify port number. If -online option isn't given\n");
	printf("         ana ignores this option. \n");
	printf("       If this option is omitted, ana connects \n");
        printf("         the target host through default port(1126)\n");
	printf("[bldfile]: Specify build data filename. ");
	printf("If the [bldfile] is - or missing,\n");
        printf("          ana reads build data from standard input.\n");
	printf("[hbfile]: Specify output hbook filename. ");
	printf("If the [hbfile] is - or missing,\n");
	printf("         ana outputs to \"hist.dat\" .\n");
	printf("-b blocksize: Specify the number of bytes of data block\n");
	printf("              If -b option is omitted, \n");
	printf("              ana uses default value(65536).\n");
	printf("              1k=1024byte.\n");
	printf("              If -online option is specified, ana detects\n");
	printf("              block size automatically.\n");
	printf("-rp parameter file: Specify run parameter file.\n");
	exit(-1);
	break;
      case 3: /* standard I/O */
		filename[j++]=NULL;
		if(j>2) {
		  printf("%s:Too many argments.\n",argv[0]);argerror();
		}
		break;
      case 4: /* use global section */
		flag|=0x8;
		if(((i+1)!=argc)&&*argv[i+1]!='-'){
		  lgname=strlen(argv[++i]);
		  if(lgname<4){
			strcpy(gname,argv[i]);
		  }
		  else{
			if(strncmp(&argv[i][lgname-4],".bld",4)!=0)
			  strcpy(gname,argv[i]);
			else i--;
		  }
		}
		lgname=strlen(gname);
		for(k=0;k<lgname;k++){
		  if((gname[k]>0x60)&&(gname[k]<0x7b)) gname[k]-=32;
		}
		break;
      case 5: /* online analysis */
		flag|=0x18;
		if(++i==argc){
		  printf("%s:Missing hostname.\n",argv[0]);
		  argerror();
		}
		strcpy(hostname,argv[i]);
		break;
      case 6: /* port option */
		flag|=0x20;
		if(++i==argc){
		  printf("%s:Missing port number.\n",argv[0]);
		  argerror();
		}
		port=atoi(argv[i]);
		if(port==0){
		  printf("%s:Illeagal port number.\n",argv[0]);
		  argerror();
		}
		break;
      case 7: /* block size option */
		if(++i==argc){
		  printf("%s:Missing block size.\n",argv[0]);
		  argerror();
		}
		else{
		  size_t largv;
		  SIZEOFBLK=atoi(argv[i]);
		  largv=strlen(argv[i]);
		  if(((*(argv[i]+largv-1))&0x4b)==0x4b){
			*(argv[i]+largv-1)=0;
			SIZEOFBLK*=1024;
		  }
		}
		if(SIZEOFBLK==0){
		  printf("%s:Illeagal block size.\n",argv[0]);
		  argerror();
		}
		break;
	  case 8: /* run parameter file */
		if(++i==argc){
		  printf("%s:Missing run parameter file.\n",argv[0]);
		  argerror();
		}
		else{
		  readpar(argv[i],&grcr);
		}
		break;
      default:
		printf("%s:Illegal option %s .\n",argv[0],argv[i]);
		argerror();
		break;
      }
    }
    else{
      filename[j++]=argv[i];
      if(j>2) {
	printf("%s:Too many argments.\n",argv[0]);argerror();
      }
    }

  }
  
  if(flag & 0x10){ /* online analysis */
    if(tcp_open(hostname,port,1,O_SAMPLE,argv[0],&buffsize)){
	  printf("%s:Fail to tcp_open %s.\n",argv[0],hostname);
	  exit(-1);
	}
	if(buffsize<0){
	  printf("%s:Inconsistent buffer size.(%d)\n",argv[0],buffsize);
	  exit(-1);
	}
	else {
	  printf("Buffer size is %d.\n",buffsize);
	  SIZEOFBLK=buffsize;
	}
  }
  else { /* online analysis with global section */
    if(filename[0]!=NULL ){
      *fd=open(filename[0],O_RDONLY,NULL);
      if(*fd == -1) {
	printf("%s:Cannnot open %s .\n",argv[0],filename[0]);
	switch(errno){
	case ENOENT:
	  printf("%s:%s:No such file!\n",argv[0],filename[0]);break;
	case EACCES:
	  printf("%s:%s:Perimission denied! \n",argv[0],filename[0]);break;
	case EISDIR:
	  printf("%s:%s is a diretory!\n",argv[0],filename[0]);break;
	default :
	  printf("%s:Error(file open)! \n",argv[0]);
	}
	printf("%s:Exit.\n",argv[0]);
	exit(-1);
      }
    }
    else *fd=STDIN_FILENO;
  }
  if(!(flag & 0x8)){  /*  offline analysis without global section */
    if(filename[1]!=NULL){
      strcpy(hbfile,filename[1]);
      printf("hbookfile:%s\n",hbfile);}
    else
      strcpy(hbfile,"hist.dat");
  }

  /* memory allocation */
  buff=(unsigned short *)malloc(SIZEOFBLK);
  rawbuff=(unsigned short *)malloc(SIZEOFBLK);
  WORDOFBLK=SIZEOFBLK/sizeof(short);

  if(buff==NULL || rawbuff==NULL){
    printf("Memory allocating error.\n");
    exit(-1);
  }
  return(flag);
}




