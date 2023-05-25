c**** histdef_b.f for ana_v1.5 or later
c**** e207 offline analysis 03/07/07

      subroutine hstdef()
      call hbook1(1,'NIM-IN-BIT',16,0.,16.,0)
      call hbook1(2,'FCET input reg.',16,0.,16.,0)


c******* Trig Scinti LAS *****/
      call hbook1(10,'LAS 1-L',512,0.,2048.,0)
      call hbook1(11,'LAS 1-R',512,0.,2048.,0)
      call hbook1(12,'LAS 2-L',512,0.,2048.,0)
      call hbook1(13,'LAS 2-R',512,0.,2048.,0)
      call hbook1(14,'LAS 3-L',512,0.,2048.,0)
      call hbook1(15,'LAS 3-R',512,0.,2047.,0)
      call hbook1(16,'LAS 4-L',512,0.,2048.,0)
      call hbook1(17,'LAS 4-R',512,0.,2048.,0)
      call hbook1(18,'LAS 5-L',512,0.,2048.,0)
      call hbook1(19,'LAS 5-R',512,0.,2048.,0)
      call hbook1(20,'LAS 6-L',512,0.,2048.,0)
      call hbook1(21,'LAS 6-R',512,0.,2048.,0)
 
      call hbook1(30,'Particle-ID LAS-1',512,0.,2048.,0)
      call hbook1(31,'Particle-ID LAS-2',512,0.,2048.,0)
      call hbook1(32,'Particle-ID LAS-3',512,0.,2048.,0)
      call hbook1(33,'Particle-ID LAS-4',512,0.,2048.,0)
      call hbook1(34,'Particle-ID LAS-5',512,0.,2048.,0)
      call hbook1(35,'Particle-ID LAS-6',512,0.,2048.,0)



c******* for FERET **************************
      call hbook1(40,'LAS 1-L TDC',512,0.,2048.,0)
      call hbook1(41,'LAS 1-R TDC',512,0.,2048.,0)
      call hbook1(42,'LAS 2-L TDC',512,0.,2048.,0)
      call hbook1(43,'LAS 2-R TDC',512,0.,2048.,0)
      call hbook1(44,'LAS 3-L TDC',512,0.,2048.,0)
      call hbook1(45,'LAS 3-R TDC',512,0.,2048.,0)
      call hbook1(46,'LAS 4-L TDC',512,0.,2048.,0)
      call hbook1(47,'LAS 4-R TDC',512,0.,2048.,0)
      call hbook1(48,'LAS 5-L TDC',512,0.,2048.,0)
      call hbook1(49,'LAS 5-R TDC',512,0.,2048.,0)
      call hbook1(50,'LAS 6-L TDC',512,0.,2048.,0)
      call hbook1(51,'LAS 6-R TDC',512,0.,2048.,0)
      call hbook1(52,'LAS-RF 1',512,0.,2048.,0)
      call hbook1(53,'LAS-RF 2',512,0.,2048.,0)
      call hbook1(54,'LAS-RF 3',512,0.,2048.,0)

      call hbook1(60,'Pos-Trig.Scinti.LAS-1',300,-500.,+100.,0)
      call hbook1(61,'Pos-Trig.Scinti.LAS-2',300,-500.,+100.,0)
      call hbook1(62,'Pos-Trig.Scinti.LAS-3',300,-500.,+100.,0)
      call hbook1(63,'Pos-Trig.Scinti.LAS-4',300,-500.,+100.,0)
      call hbook1(64,'Pos-Trig.Scinti.LAS-5',300,-500.,+100.,0)
      call hbook1(65,'Pos-Trig.Scinti.LAS-6',300,-500.,+100.,0)


c******* for LAS 3377 **************************
      call hbook1(90,'LAS-FR-U channel',280,-1.,279.,0)
      call hbook1(91,'LAS-FR-V channel',280,-1.,279.,0)
      call hbook1(92,'LAS-RE-U channel',280,-1.,279.,0)
      call hbook1(93,'LAS-RE-V channel',280,-1.,279.,0)
      call hbook1(94,'LAS-FR-U time',1024,0.,1024.,0)
      call hbook1(95,'LAS-FR-V time',1024,0.,1024.,0)
      call hbook1(96,'LAS-RE-U time',1024,0.,1024.,0)
      call hbook1(97,'LAS-RE-V time',1024,0.,1024.,0)
      call hbook1(98,'LAS-FR-U hits/clust',20,0.,20.,0)
      call hbook1(99,'LAS-FR-V hits/clust',20,0.,20.,0)
      call hbook1(100,'LAS-RE-U hits/clust',20,0.,20.,0)
      call hbook1(101,'LAS-RE-V hits/clust',20,0.,20.,0)
      call hbook1(102,'LAS-FR-U clust/chamber',20,0.,20.,0)
      call hbook1(103,'LAS-FR-V clust/chamber',20,0.,20.,0)
      call hbook1(104,'LAS-RE-U clust/chamber',20,0.,20.,0)
      call hbook1(105,'LAS-RE-V clust/chamber',20,0.,20.,0)
      call hbook1(106,'LAS-FR-U POS-ID',20,0.,20.,0)
      call hbook1(107,'LAS-FR-V POS-ID',20,0.,20.,0)
      call hbook1(108,'LAS-RE-U POS-ID',20,0.,20.,0)
      call hbook1(109,'LAS-RE-V POS-ID',20,0.,20.,0)
      call hbook1(110,'LAS-FR-U dirft-length',120,-0.1,1.1,0)
      call hbook1(111,'LAS-FR-V dirft-length',120,-0.1,1.1,0)
      call hbook1(112,'LAS-RE-U dirft-length',120,-0.1,1.1,0)
      call hbook1(113,'LAS-RE-V dirft-length',120,-0.1,1.1,0)
      call hbook1(114,'LAS-FR-U position',1600,-800.,800.,0)
      call hbook1(115,'LAS-FR-V position',1600,-800.,800.,0)
      call hbook1(116,'LAS-RE-U position',1600,-600.,1000.,0)
      call hbook1(117,'LAS-RE-V position',1600,-600.,1000.,0)
      call hbook1(118,'LAS-FR-U dis-fx',260,-1.,1.,0)
      call hbook1(119,'LAS-FR-V dis-fu',260,-1.,1.,0)
      call hbook1(120,'LAS-RE-U dis-rx',260,-1.,1.,0)
      call hbook1(121,'LAS-RE-V dis-ru',260,-1.,1.,0)
      call hbook1(122,'LAS-VDC RAY-ID',26,0.,26.,0)

      call hbook1(123,'LAS-MWDC-X Ch',64,0.,64.,0)
      call hbook1(124,'LAS-MWDC-X T',1024,0.,1024.,0)
      call hbook1(125,'LAS-MWDC-Y Ch',64,0.,64.,0)
      call hbook1(126,'LAS-MWDC-Y T',1024,0.,1024.,0)
      call hbook2(127,'LAS-MWDC NHIT',8,0.,8.,17,0.,17.,0)
      call hbook2(128,'LAS-MWDC NCLU',8,0.,8.,10,0.,10.,0)
      call hbook2(350,'theta-x-c vs X1',50,-5.,5.,16,0.,16.,0)
      call hbook2(351,'theta-x-c vs X2',50,-5.,5.,16,0.,16.,0)
      call hbook2(352,'theta-x-c vs X3',50,-5.,5.,16,0.,16.,0)
      call hbook2(353,'theta-x-c vs X4',50,-5.,5.,16,0.,16.,0)
      call hbook2(354,'theta-y-c vs Y1',50,-5.,5.,16,0.,16.,0)
      call hbook2(355,'theta-y-c vs Y2',50,-5.,5.,16,0.,16.,0)
      call hbook2(356,'theta-y-c vs Y3',50,-5.,5.,16,0.,16.,0)
      call hbook2(357,'theta-y-c vs Y4',50,-5.,5.,16,0.,16.,0)

      call hbook1(130,'Position X10-LAS',1800,-600.,600.,0)
      call hbook1(131,'Position Y10-LAS',600,-150.,150.,0)
      call hbook1(132,'theta-x LAS',400,-20.,20.,0)
      call hbook1(133,'theta-y LAS',400,-10.,10.,0)
      call hbook1(134,'X LAS',1200,-20.,20.,0)
      call hbook1(135,'Y LAS',500,-5.,5.,0)
      call hbook1(136,'theta-x-c LAS',400,-5.,5.,0)
      call hbook1(137,'theta-y-c LAS',480,-12.,12.,0)
      call hbook1(138,'LAS-RF-C',2048,0.,2048.,0)

      call hbook2(300,'X10 vs Y10(VDC)',200,-600.,600.,100,-150.,150.,0)
      call hbook2(301,'X10 vs theta-x',200,-600.,600.,100,-20.,20.,0)
      call hbook2(302,'theta-x vs theta-y',100,-20.,20.,120,-10.
     +,10.,0)
      call hbook2(303,'X10 vs theta-x-c',200,-600.,600.,100,-6.,6.,0)
      call hbook2(304,'X vs theta-x-c',200,-20.,20.,100,-6.,6.,0)
      call hbook2(305,'Y10 vs theat-y',150,-150.,150.,
     +100,-5.,5.,0)
      call hbook2(306,'theta-x-c vs theta-y-c',100,-5.,5.,100,-5.,5.,0)
      call hbook2(307,'Y vs theat-x-c',200,-5.,5.,
     +100,-6.,6.,0)

c*** for Trig. Scinti. (LAS) **********************
      call hbook2(320,'X10 vs PID LAS-1',100,-600.,600.,128,0.,2048.,0)
      call hbook2(321,'X10 vs PID LAS-2',100,-600.,600.,128,0.,2048.,0)
      call hbook2(322,'X10 vs PID LAS-3',100,-600.,600.,128,0.,2048.,0)
      call hbook2(323,'X10 vs PID LAS-4',100,-600.,600.,128,0.,2048.,0)
      call hbook2(324,'X10 vs PID LAS-5',100,-600.,600.,128,0.,2048.,0)
      call hbook2(325,'X10 vs PID LAS-6',100,-600.,600.,128,0.,2048.,0)

      call hbook2(360,'X10 vs PID LAS-1',100,-600.,600.,128,0.,2048.,0)
      call hbook2(361,'X10 vs PID LAS-2',100,-600.,600.,128,0.,2048.,0)
      call hbook2(362,'X10 vs PID LAS-3',100,-600.,600.,128,0.,2048.,0)
      call hbook2(363,'X10 vs PID LAS-4',100,-600.,600.,128,0.,2048.,0)
      call hbook2(364,'X10 vs PID LAS-5',100,-600.,600.,128,0.,2048.,0)
      call hbook2(365,'X10 vs PID LAS-6',100,-600.,600.,128,0.,2048.,0)

      call hbook2(330,'X10 vs LAS-1L',100,-600.,600.,128,0.,2048.,0)
      call hbook2(331,'X10 vs LAS-2L',100,-600.,600.,128,0.,2048.,0)
      call hbook2(332,'X10 vs LAS-3L',100,-600.,600.,128,0.,2048.,0)
      call hbook2(333,'X10 vs LAS-4L',100,-600.,600.,128,0.,2048.,0)
      call hbook2(334,'X10 vs LAS-5L',100,-600.,600.,128,0.,2048.,0)
      call hbook2(335,'X10 vs LAS-6L',100,-600.,600.,128,0.,2048.,0)
      
      call hbook2(340,'X10 vs LAS-1R',100,-600.,600.,128,0.,2048.,0)
      call hbook2(341,'X10 vs LAS-2R',100,-600.,600.,128,0.,2048.,0)
      call hbook2(342,'X10 vs LAS-3R',100,-600.,600.,128,0.,2048.,0)
      call hbook2(343,'X10 vs LAS-4R',100,-600.,600.,128,0.,2048.,0)
      call hbook2(344,'X10 vs LAS-5R',100,-600.,600.,128,0.,2048.,0)
      call hbook2(345,'X10 vs LAS-6R',100,-600.,600.,128,0.,2048.,0)

      

c*** Npol  **********************
      call hbook1(200,'NPOL-RF',512,0.,2048.,0.)
      call hbook2(201,'NPOL-ADC',54,0,54,512,0.,2048.,0)
c      call hbook2(202,'NPOL-TDC',54,0,54,512,0.,2048.,0)
      call hbook2(202,'NPOL-TDC',54,0,54,2048,0.,2048.,0)
      call hbook2(203,'NPOL-PID',27,0,27,512,0.,2048.,0)
      call hbook2(204,'NPOL-POS',27,0,27,400,-200.,200.,0)
      call hbook2(205,'NPOL-T',54,0,54,480,-10.,110.,0)
      call hbook2(206,'NPOL-TOF',27,0,27,100,-100.,100.,0)
      call hbook2(207,'NPOL-GR-TDC',27,0,27,512,0.,2048.,0)

      return  
      
      end
