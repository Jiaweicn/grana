/* 00/01/21  modify howto decide return values. */

int go_up(short *time,int numofwire ,int currentwire);
int go_down(short *time,int numofwire ,int currentwire);


int go_down(short *time,int numofwire ,int currentwire) {
  int i;
  if(currentwire<numofwire){
    if(*(time+currentwire-1)>=*(time+currentwire)){
      currentwire++;
      switch(i=go_up(time,numofwire,currentwire)) {
      case 1:
	currentwire-=2;
	return(currentwire);
	break;
      default:
	return(i);
      }
    }
    else {
      currentwire++;
      return(go_down(time,numofwire,currentwire));
    }
  }
  else {
    return(currentwire-1); /* no peak */
  }
}
  
int go_up(short *time,int numofwire ,int currentwire) {
  while(currentwire<numofwire){
    if(*(time+currentwire-1)<=*(time+currentwire)) return(-2);/* if two peak */
    currentwire++;
  }
  return(1);
}
