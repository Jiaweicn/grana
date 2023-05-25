/* e207 *****************************************/
/* 03/07/07 T.Kawabata */
/* run1081 (13deg 12C Ela/Inela, Tensor) */
/*
   vdcpos[9][]   position in focal plane by VDC
   vdcpos10_c[]  position by VDC after correction
         for above two vectors
	   [0] x; [1] y; [2] z;
   theta_x[],theta_y[]   scattering angle
           [0] no correction
           [1] after correction
           [2] on FPP carbon block
   ex_energy     excitation energy
   rf_c          timing data against cyclotron RF after correction
   grcf        run parameters
*/

switch(grcr.igrmode){
 case 0:
   theta_y[1]=theta_y[0]*GR_YMAG;
   theta_y[2]=0;
   vdcpos10_c[0]=vdcpos[9][0]*grcr.dpdx*100.0;
   vdcpos10_c[1]=(vdcpos[9][1]-4.0*theta_y[0])/2.0;
   if(1){
     static double aparm[4][2]={
       {    -.45441E-01,    -.42280    },
       {    -.35752E-03,    -.86843E-04},
       {     .11521E-05,     .77804E-07},
       {    -.76249E-09,     .10207E-08}};
     static double xparm[4][2]={
       {    0.0,            0.0 },
       {     .77728E-01,    -.75689E-02},
       {     .34843E-01,    -.29263E-02},
       {     .15469E-01,    -.56883E-02}};
     double ppar[4];
     int iloop;
     /*** Correct A ***/
     theta_x_ray=theta_x[0]-(0.52302e-2*vdcpos[9][0]+1.7311);
     for(iloop=1;iloop<4;iloop++){
       ppar[iloop]=aparm[iloop][0]+aparm[iloop][1]*theta_x_ray;
     }
     theta_x[1]=
       (theta_x_ray-
	(((ppar[3]*vdcpos[9][0]+ppar[2])*vdcpos[9][0])+ppar[1])*vdcpos[9][0])
       *aparm[0][1]+aparm[0][0];
     theta_x[1]/=1.006;
   
     /*** Correct X ***/
     for(iloop=1;iloop<4;iloop++){
       ppar[iloop]=xparm[iloop][0]+xparm[iloop][1]*vdcpos10_c[0];
     }
     vdcpos10_c[0]-=(((ppar[3]*theta_x[1]+ppar[2])*theta_x[1])
		   +ppar[1])*theta_x[1];
   }
   break;
 case 1:
   grmat(vdcpos[9],vdcpos10_c,theta_x,theta_y);
   /*   theta_y[1]-=(-1.7082/0.8671*grcr.ytgt);*/
   theta_y[2]=theta_y[1];
   break;
}

vdcpos10_c[1]-=grcr.ytgt;
p3lab=(vdcpos10_c[0]/100+1.0)*grcr.brhogr;
K3=sqrt(grcr.m3*grcr.m3+p3lab*p3lab)-grcr.m3;
if(1){
  double tmpz,tmpr,tmpxr,tmpyr,tmpthr;
  tmpxr=(theta_x[1]+grcr.grang)*D_TO_R;
  tmpyr=theta_y[2]*D_TO_R;
  tmpz=cos(tmpxr);
  tmpr=sqrt(1+pow(tmpz*tan(tmpyr),2.0));
  tmpthr=acos(tmpz/tmpr);
  theta_x[2]=tmpthr/D_TO_R-grcr.grang;
  ex_energy=calcex4(tmpthr,grcr.m1,grcr.m2,grcr.m3,grcr.m4,grcr.K1,K3);
}
ex_energy*=grcr.dex[1];
ex_energy+=grcr.dex[0];

rf_c=feret_gr[4]-43.335*theta_x[1]-0.099591*vdcpos[9][0];
rf_c-=(5.4970*theta_x[1]-1.5828*theta_x[1]*theta_x[1]);







