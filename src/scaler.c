/* scalar.c
   00/01/06  Ver.1.1 increase number of scaler to 4
   00/01/13  Ver.1.2 define max value SC_MAX in one block
*/
#include<stdio.h>
#include "ana10.h"

extern int rp,wp,scalerno;
extern unsigned long block;
extern unsigned short regionsize,*buff;
extern unsigned int scaler[N_SCA][16];

int camac_scaler() {
  int i=0;
  unsigned short data1,data2;
  if(regionsize>32){
    printf("b %ld:Too large scaler regionsize! (0x%4x)\n",block,regionsize);
    exit(-1);
  }
  if(scalerno>3){
    printf("Five scaler region exist in one block!\n");
    exit(-1);
  }
  rp=wp+regionsize;
  while(wp<rp){
    int scvalue;
    data1=buff[++wp];
    data2=buff[++wp];
    scvalue=data1+data2*256;
    if(scvalue>SC_MAX) {
      printf("b %ld:sc %d:ch %d: Too Large scaler count! 0x%8x\n"
	     ,block,scalerno,i,scvalue);
    }
    scaler[scalerno][i++]+=scvalue;
  }
  scalerno++;
  return(0);
}
