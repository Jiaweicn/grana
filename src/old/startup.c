/* for ana_v0.71
       startup routine 引数の処理をおこなう。*/
/*  00/01/24 block size に k(=1024) を許す  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "router.h"
#include "ana9.h"

#define BLD 1

extern unsigned short *buff,*rawbuff;
extern size_t SIZEOFBLK,WORDOFBLK;

void argerror(){
  printf
  ("Usage: ana [-vdc] [-help] [-global] [-online hostname] [-port port] [bldfile] [hbfile]\n");
  exit(-1);
}

void drifttime(){}

int check_option(char *arg){
  const struct {char *option; int i;} 
  table[]={{"-vdc",1},{"-help",2},{"-",3},{"-global",4},{"-online",5},
	 {"-port",6},{"-b",7}};
  int j;
  for(j=0;j<7;j++){ 
  /* when you add new option, you MUST change an above line.*/
    if(strcmp(arg,table[j].option)==0)
      return(table[j].i);
  }
  return(-1);
}


int startup(int argc,char *argv[],int *fd,char *hbfile) {
  int i,j,flag,port=0;
  int buffsize;
  char *filename[2];
  char hostname[80];
  flag=0;j=0;
  filename[0]=NULL;filename[1]=NULL;

  SIZEOFBLK=BUFF_SIZE;    /* set default block size */
  for(i=1;i<argc;i++){
    if(*(argv[i])=='-'){
      switch(check_option(argv[i])){
      case 1: /* vdc calibration */
	flag|=0x1;
	break;
      case 2: /* help */
	printf("Usage: ana [-vdc] [-help] [-global] [-online hostname] [-port port] [bldfile] [hbfile] [-b blocksize]\n");
	printf("-vdc: Ana makes VDC time-position table.\n");
	printf("-help: Display this help.\n");
	printf("-global: Use global section.\n");
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
	  if(*(argv[i]+largv-1)&0x4b==0x4b){
	    *(argv[i]+largv-1)=0;
	    SIZEOFBLK*=1024;
	  }
	}
	if(SIZEOFBLK==0){
	  printf("%s:Illeagal block size.\n",argv[0]);
	  argerror();
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




