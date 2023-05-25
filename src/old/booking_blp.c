/** booking_blp.c 00/01/06 ***********************************/
/** e137(0pol-SDR) online analysis of BLP ************/

/** INPUT_REG ***************************************************/
for(j=0;j<16;j++){
  if(eventid[j]!=0) {
    book1(1,(float)j);
  }
}


/** INPUT_REG FCET ***************************************************/
for(j=0;j<16;j++){
  if(fcet_id[j]!=-1) {
    book1(2,(float)j);}
}

/** ADC ***********************************************************/
histid1=100;
for(channel=0;channel<16;channel++) {
  if(eventid[SPIN_UP]) book1(histid1+channel,(float)adc[channel]);
  if(eventid[SPIN_DOWN]) book1(histid1+channel+16,(float)adc[channel]);
}
     
/** TDC ***********************************************************/
histid1=200;
for(channel=0;channel<16;channel++){
  if(eventid[SPIN_UP]) book1(histid1+channel,(float)tdc[channel]);
  if(eventid[SPIN_DOWN]) book1(histid1+channel+16,(float)tdc[channel]);
}


