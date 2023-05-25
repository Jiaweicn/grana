#include <stdio.h>
#include <ana10.h>

extern unsigned long block;
extern int eventcount,fsizecount,fieldsize;
extern unsigned short regionid,regionsize,*buff;
extern int wp;
extern short eventid[16],fcet_id[16];
extern unsigned long eventerror;

int event_id(){
  unsigned short data;
  short j;

  data=buff[++wp];
  regionid=(data>>12);
  regionsize=(data & 0x0fff);
  /*printf(" RegionID = %d  RegionSIZE = %d \n",
    regionid,regionsize);*/
  if(regionid==INPUT_REG) { /* Event ID*/
    fsizecount+=(regionsize+1);
    data=buff[++wp];
/* printf(" Input Reg %x \n",data);*/
    for(j=0;j<16;j++) {
      if(data & 1) {eventid[j]=1;}
      data=data>>1;}
/*for(j=0;j<16;j++) {
  printf("event ID:%d\t%d\n",j,eventid[j]);}*/
  }
  else {
    printf("b:%ld e:%d:There is no Event ID at head of eventdata!\n",
	   block,eventcount);
    eventerror++;
    /*tmp*/
    fsizecount=fieldsize;
    wp+=(fieldsize-1);
    /*tmp*/
	 return(-1);
  }
  return(0);
}


int input_reg()
{
  int k;
  unsigned short data;
  data=buff[++wp];
  /* printf(" Input Reg %x \n",data);*/
  for(k=0;k<16;k++) {
    if(data & 1) {fcet_id[k]=1;}
    data=data>>1;}
/*  for(k=0;k<16;k++) {
    printf("event ID:%d\t%d\n",k,fcet_id[k]);}*/

return(0);
}




