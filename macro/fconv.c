#include <stdio.h>

void main(){
  char buf[100];
  double a,b,c;
  int i;
  fgets(buf,100,stdin);
  fputs(buf,stdout);
  while(fgets(buf,100,stdin)!=NULL){
    sscanf(buf,"%lf %lf %lf",&a,&b,&c);
    i=(int)a;
    fprintf(stdout,"%4d    %11.4e    %11.4e\n",i,b,c);

  }

}   
