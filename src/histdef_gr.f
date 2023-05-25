c**** histdef_nt.f for ana_v1.5 or later
c**** e253 offline analysis with ntuple 05/10/28

      subroutine hstdef()
      common /CMTRK/xyab(13)
      character*3 chtags(13)
      data chtags/'X','Y','A','B','LA1','RA1','LA2','RA2',
     +'LT1','RT1','LT2','RT2','RF'/

      call hbook1(1,'NIM-IN-BIT',16,0.,16.,0)
      call hbook1(2,'FCET input reg.',16,0.,16.,0)

c******* for FERA **************************
      call hbook1(10,'Trig. Scinti. 1-L ',2048,0.,2048.,0)
      call hbook1(11,'Trig. Scinti. 1-R ',2048,0.,2048.,0)
      call hbook1(12,'Trig. Scinti. 2-L',2048,0.,2048.,0)
      call hbook1(13,'Trig. Scinti. 2-R',2048,0.,2048.,0)


c******* for FERET **************************
      call hbook1(14,'Trig. Scinti. 1-L TDC',2048,0.,2048.,0)
      call hbook1(15,'Trig. Scinti. 1-R TDC',2048,0.,2048.,0)
      call hbook1(16,'Trig. Scinti. 2-L TDC',2048,0.,2048.,0)
      call hbook1(17,'Trig. Scinti. 2-R TDC',2048,0.,2048.,0)
      call hbook1(18,'GR-RF',2048,0.,2048.,0)


c******* for 3377 **************************
      call hbook1(90,'GR-FR-X channel',200,0.,200.,0)
      call hbook1(91,'GR-FR-U channel',240,0.,240.,0)
      call hbook1(92,'GR-RE-X channel',200,0.,200.,0)
      call hbook1(93,'GR-RE-U channel',240,0.,240.,0)
      call hbook1(94,'GR-FR-X time',1024,0.,1024.,0)
      call hbook1(95,'GR-FR-U time',1024,0.,1024.,0)
      call hbook1(96,'GR-RE-X time',1024,0.,1024.,0)
      call hbook1(97,'GR-RE-U time',1024,0.,1024.,0)
      call hbook1(98,'GR-FR-X hits/clust',20,0.,20.,0)
      call hbook1(99,'GR-FR-U hits/clust',20,0.,20.,0)
      call hbook1(100,'GR-RE-X hits/clust',20,0.,20.,0)
      call hbook1(101,'GR-RE-U hits/clust',20,0.,20.,0)
      call hbook1(102,'GR-FR-X clust/chamber',20,0.,20.,0)
      call hbook1(103,'GR-FR-U clust/chamber',20,0.,20.,0)
      call hbook1(104,'GR-RE-X clust/chamber',20,0.,20.,0)
      call hbook1(105,'GR-RE-U clust/chamber',20,0.,20.,0)
      call hbook1(106,'GR-FR-X POS-ID',20,0.,20.,0)
      call hbook1(107,'GR-FR-U POS-ID',20,0.,20.,0)
      call hbook1(108,'GR-RE-X POS-ID',20,0.,20.,0)
      call hbook1(109,'GR-RE-U POS-ID',20,0.,20.,0)
      call hbook1(110,'GR-FR-X drift-length',120,-0.1,1.1,0)
      call hbook1(111,'GR-FR-U drift-length',120,-0.1,1.1,0)
      call hbook1(112,'GR-RE-X drift-length',120,-0.1,1.1,0)
      call hbook1(113,'GR-RE-U drift-length',120,-0.1,1.1,0)
      call hbook1(114,'GR-FR-X position',1300,-700.,600.,0)
      call hbook1(115,'GR-FR-U position',1300,-700.,600.,0)
      call hbook1(116,'GR-RE-X position',1400,-1000.,400.,0)
      call hbook1(117,'GR-RE-U position',1400,-1000.,400.,0)
      call hbook1(118,'GR-FR-X dis-fx',260,-1.,1.,0)
      call hbook1(119,'GR-FR-U dis-fu',260,-1.,1.,0)
      call hbook1(120,'GR-RE-X dis-rx',260,-1.,1.,0)
      call hbook1(121,'GR-RE-U dis-ru',260,-1.,1.,0)
      call hbook1(122,'GR-VDC RAY-ID',26,0.,26.,0)
      call hbook1(123,'GR-VDC RAY-ID up',26,0.,26.,0)
      call hbook1(124,'GR-VDC RAY-ID dn',26,0.,26.,0)

c******* for 3377 **************************
      call hbook1(130,'Position X10-VDC',2000,-500.,500.,0)
      call hbook1(131,'Position Y10-VDC',480,-50.,50.,0)
      call hbook1(132,'theta-x',400,-10.,10.,0)
      call hbook1(133,'theta-y',400,-5.,5.,0)
      call hbook1(134,'X',2000,-3.,3.,0)
      call hbook1(135,'Y',500,-25.,25.,0)
      call hbook1(136,'theta-x-c',400,-5.,5.,0)
      call hbook1(137,'theta-y-c',480,-12.,12.,0)
      call hbook1(138,'GR-RF-C',2048,0.,2048.,0)
      call hbook1(139,'GR-RF-C rf-cut',2048,0.,2048.,0)

      call hbook2(300,'X10 vs Y10(VDC)',200,-500.,500.,100,-50.,50.,0)
      call hbook2(301,'X10 vs theta-x',200,-500.,500.,100,-10.,10.,0)
      call hbook2(302,'theta-x vs theta-y',100,-10.,10.,120,-4.
     +,4.,0)
      call hbook2(303,'Y10 vs theta-y',200,-50.,50.,100,-5.,5.,0)
      call hbook2(304,'X vs theta-x-c',200,-3.,3.,100,-2.,2.,0)

      call hbook2(305,'X vs Y',200,-3.,3.,100,-25.,25.,0)
      call hbook2(306,'X vs theat-y-c',200,-3.,3.,100,-10.,10.,0)
      call hbook2(307,'theta-x-c vs theta-y-c',90,-3.,3.,90,-10.,10.,0)

      call hbook2(309,'X10 vs RF',200,-500.,500.,200,200.,400.,0)

      call hbook2(310,'theta-x-c vs RF',180,-2.,2.,200,200.,400.,0)

c**** th-cut

      call hbook1(140,'X th-cut',2000,-3.,3.,0)
      call hbook1(141,'Y th-cut',500,-50.,50.,0)

      call hbook2(312,'X vs PI-1L th-cut',100,-3.,3.,
     +60,0.,800.,0)
      call hbook2(313,'X vs PI-1R th-cut',100,-3.,3.,
     +60,0.,800.,0)
      call hbook2(314,'X vs PID-1 th-cut',100,-3.,3.,
     +60,0.,800.,0)
      call hbook2(315,'X vs PI-2L th-cut',100,-3.,3.,
     +60,0.,800.,0)
      call hbook2(316,'X vs PI-2R th-cut',100,-3.,3.,
     +60,0.,800.,0)
      call hbook2(317,'X vs PID-2 th-cut',100,-3.,3.,
     +60,0.,800.,0)

c**** y-cut
      call hbook1(142,'X th-cut y-down',2400,-3.,3.,0)
      call hbook1(143,'Y th-cut y-down',500,-50.,50.,0)
      call hbook1(144,'X th-cut y-peak',2400,-3.,3.,0)
      call hbook1(145,'Y th-cut y-peak',500,-50.,50.,0)
      call hbook1(146,'X th-cut y-up',2400,-3.,3.,0)
      call hbook1(147,'Y th-cut y-up',500,-50.,50.,0)
      call hbook1(148,'X th-cut y-back',2400,-3.,3.,0)
      call hbook1(149,'Y th-cut y-back',500,-50.,50.,0)

c*** for Run Stability
      call hbook2(334,'Ex vs Block',100,3.,5.,100,0.,40960.,0)

c*** Excitation energy
      call hbook2(500,'Ex PI yp',2000,-2.,38.,4,-0.8,0.8,0)
      call hbook2(501,'Ex PI yb',2000,-2.,38.,4,-0.8,0.8,0)
      call hbook1(5,'Ex th-cut y-peak',2000,-2.,38.,0)
      call hbook1(6,'Ex th-cut y-back',2000,-2.,38.,0)


c*** for Trig. Scinti. ************************
      call hbook1(180,'Position-Trig.Scinti1.TDC',240,-140.,+100.,0)
      call hbook1(181,'Position-Trig.Scinti2 .TDC',240,-140.,+100.,0)
      call hbook1(182,'Particle-ID1',400,0.,800.,0)
      call hbook1(183,'Particle-ID2',400,0.,800.,0)
      call hbook1(184,'Particle-ID1 rf-cut',400,0.,800.,0)
      call hbook1(185,'Particle-ID2 rf-cut',400,0.,800.,0)


c**** VDC EFF Check

c******* VDC EFF CHECK
      call hbook2(600,'GR-FR-X theta-x',100,-10.,20.,100,-10.,20.,0)
      call hbook2(601,'GR-FR-U theta-x',100,-10.,20.,100,-10.,20.,0)
      call hbook2(602,'GR-RE-X theta-x',100,-10.,20.,100,-10.,20.,0)
      call hbook2(603,'GR-RE-U theta-x',100,-10.,20.,100,-10.,20.,0)

      call hbook2(604,'GR-FR-X FPPOS',100,-500.,400.,100,-500.,400.,0)
      call hbook2(605,'GR-FR-U FPPOS',100,-500.,400.,100,-500.,400.,0)
      call hbook2(606,'GR-RE-X FPPOS',100,-500.,400.,100,-500.,400.,0)
      call hbook2(607,'GR-RE-U FPPOS',100,-500.,400.,100,-500.,400.,0)

      call hbook1(608,'GR-FR-X FPPOS-diff',200,-100.,100.,0)
      call hbook1(609,'GR-RE-X FPPOS-diff',200,-100.,100.,0)
      call hbook1(610,'GR-FX theta-c-diff',200,-1.,1.,0)
      call hbook1(611,'GR-RX theta-c-diff',200,-1.,1.,0)

      call hbook1(612,'GR-FR-X Y-diff',100,-20.,20.,0)
      call hbook1(613,'GR-FR-U Y-diff',100,-20.,20.,0)
      call hbook1(614,'GR-RE-X Y-diff',100,-20.,20.,0)
      call hbook1(615,'GR-RE-U Y-diff',100,-20.,20.,0)

      call hbook1(616,'GR-FR-X Ex-diff',100,-1.,1.,0)
      call hbook1(617,'GR-FR-U Ex-diff',100,-1.,1.,0)
      call hbook1(618,'GR-RE-X Ex-diff',100,-1.,1.,0)
      call hbook1(619,'GR-RE-U Ex-diff',100,-1.,1.,0)

      call hbook2(620,'EX vs posids',200,-2.,38.,8,0.,8.,0)
      call hbook2(630,'EX vs posids thcut',200,-2.,38.,8,0.,8.,0)


c*** Ntuple
      call hbooknc(700,'XYAB',13,'Block1',xyab,chtags)

      return  

      end
