c**** histdef_nt.f for ana_v1.5 or later
c**** e253 offline analysis with ntuple 05/10/28

      subroutine hstdef()
      common /CMTRK/xyab(8)
      character*2 chtags(8)
      data chtags/'X','Y','A','B','XC','YC','AC','BC'/

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
      call hbook2(303,'X10 vs theta-x-c',200,-500.,500.,100,-6.,6.,0)
      call hbook2(304,'X vs theta-x-c',200,-3.,3.,100,-2.,2.,0)

      call hbook2(305,'X vs Y',200,-3.,3.,100,-25.,25.,0)
      call hbook2(306,'X vs theta-y-c',200,-3.,3.,100,-5.,5.,0)
      call hbook2(307,'Y vs theat-y-c',300,-25.,25.,
     +100,-10.,10.,0)

      call hbook2(308,'theta-x-c vs theta-y-c',90,-3.,3.,90,-3.,3.,0)
      call hbook2(309,'X10 vs RF',200,-500.,500.,200,500.,1000.,0)
      call hbook2(310,'theta-x-c vs RF',180,-2.,2.,200,500.,750.,0)

c**** th-cut

      call hbook1(140,'X th-cut',2000,-3.,3.,0)
      call hbook1(141,'Y th-cut',500,-15.,15.,0)
      call hbook2(311,'X vs theat-y-c th-cut',200,-3.,3.,100,-5.,5.,0)
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
c      call hbook2(318,'Ex vs theta-x th-cut yt pi',100,-2.,38.,
c     +60,-7.,7.,0)
c      call hbook2(319,'Ex vs theta-x th-cut yb pi',100,-2.,38.,
c     +60,-7.,7.,0)
      call hbook2(320,'X vs theta-c g',200,-3.,3.,120,-2.,4.,0)
c      call hbook2(321,'Ex vs theta-c b',200,0.,40.,120,-2.,4.,0)
      call hbook2(321,'Ex vs theta-c b',200,1.,9.,120,-2.,4.,0)
c      call hbook2(321,'Ex vs theta-c b',200,7.,13.,120,-2.,4.,0)
      call hbook2(322,'Ex vs theta-c b',200,12.,18.,120,-2.,4.,0)
c      call hbook2(322,'PID-1 vs PID-2',100,0.,800.,
c     +100,0.,800.,0)

      call hbook2(330,'theta-x-c vs theta-y-c',90,-3.,3.,90,-3.,3.,0)
      call hbook2(331,'theta-x vs Y10',120,-6.,6.,90,-30.,30.,0)
      call hbook1(150,'X delta-cut',2400,-3.,3.,0)
      call hbook1(151,'Y delta-cut',500,-40.,40.,0)
      call hbook1(152,'Ex th-cut y-peak',2000,-2.,38.,0)

c**** y-cut
      call hbook1(142,'X th-cut y-down',2400,-3.,3.,0)
      call hbook1(143,'Y th-cut y-down',500,-15.,15.,0)
      call hbook1(144,'X th-cut y-peak',2400,-3.,3.,0)
      call hbook1(145,'Y th-cut y-peak',500,-15.,15.,0)
      call hbook1(146,'X th-cut y-up',2400,-3.,3.,0)
      call hbook1(147,'Y th-cut y-up',500,-15.,15.,0)
      call hbook1(148,'X th-cut y-back',2400,-3.,3.,0)
      call hbook1(149,'Y th-cut y-back',500,-15.,15.,0)

c*** for Recoil Counter gate
c      call hbook1(150,'Theta-x-c pp',100,-2.,2.,0)
c      call hbook1(151,'Theta-x-c ppg',100,-2.,2.,0)
c      call hbook1(152,'Theta-x-c ppb',100,-2.,2.,0)
c      call hbook2(330,'X vs theta-x-c pp',200,-3.,3.,100,-2.,2.,0)
c      call hbook2(331,'X vs theta-x-c ppg',200,-3.,3.,100,-2.,2.,0)
c      call hbook2(332,'X vs theta-x-c ppb',200,-3.,3.,100,-2.,2.,0)

c*** for Run Stability
c      call hbook2(333,'Y vs Block',100,-5.,5.,100,0.,40960.,0)
      call hbook2(334,'Ex vs Block',100,3.,5.,100,0.,40960.,0)

c*** Excitation energy
      call hbook2(500,'Ex PI yp',2000,-2.,38.,4,-0.8,0.8,0)
      call hbook2(501,'Ex PI yb',2000,-2.,38.,4,-0.8,0.8,0)
      call hbook1(5,'Ex th-cut y-peak',2000,-2.,38.,0)
      call hbook1(6,'Ex th-cut y-back',2000,-2.,38.,0)
      call hbook1(502,'Ex th-cut y-peak',2000,-2.,38.,0)
      call hbook1(503,'Ex th-cut y-back',2000,-2.,38.,0)

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

c******* SSD **************************
c      call hbook1(392,'SSD 1',4096,1.,4097.,0)
c      call hbook1(393,'SSD 2',4096,1.,4097.,0)
c      call hbook1(394,'SSD 3',4096,1.,4097.,0)
c      call hbook1(395,'SSD 4',4096,1.,4097.,0)
c      call hbook1(396,'SSD 1 nhit',16,0.,16.,0)
c      call hbook1(397,'SSD 2 nhit',16,0.,16.,0)
c      call hbook1(398,'SSD 3 nhit',16,0.,16.,0)
c      call hbook1(399,'SSD 4 nhit',16,0.,16.,0)

      call hbook2(412,'SSD', 32,0.,32.,256,1.,4097.,0)
      call hbook2(413,'SSD', 32,0.,32.,300,0.,15.,0)

      call hbook1(800,'SSD 00',4096,1.,4097.,0)
      call hbook1(801,'SSD 01',4096,1.,4097.,0)
      call hbook1(802,'SSD 02',4096,1.,4097.,0)
      call hbook1(803,'SSD 03',4096,1.,4097.,0)
      call hbook1(804,'SSD 04',4096,1.,4097.,0)
      call hbook1(805,'SSD 05',4096,1.,4097.,0)
      call hbook1(806,'SSD 06',4096,1.,4097.,0)
      call hbook1(807,'SSD 07',4096,1.,4097.,0)
      call hbook1(808,'SSD 08',4096,1.,4097.,0)
      call hbook1(809,'SSD 09',4096,1.,4097.,0)
      call hbook1(810,'SSD 0A',4096,1.,4097.,0)
      call hbook1(811,'SSD 0B',4096,1.,4097.,0)
      call hbook1(812,'SSD 0C',4096,1.,4097.,0)
      call hbook1(813,'SSD 0D',4096,1.,4097.,0)
      call hbook1(814,'SSD 0E',4096,1.,4097.,0)
      call hbook1(815,'SSD 0F',4096,1.,4097.,0)
      call hbook1(816,'SSD 10',4096,1.,4097.,0)
      call hbook1(817,'SSD 11',4096,1.,4097.,0)
      call hbook1(818,'SSD 12',4096,1.,4097.,0)
      call hbook1(819,'SSD 13',4096,1.,4097.,0)
      call hbook1(820,'SSD 14',4096,1.,4097.,0)
      call hbook1(821,'SSD 15',4096,1.,4097.,0)
      call hbook1(822,'SSD 16',4096,1.,4097.,0)
      call hbook1(823,'SSD 17',4096,1.,4097.,0)
      call hbook1(824,'SSD 18',4096,1.,4097.,0)
      call hbook1(825,'SSD 19',4096,1.,4097.,0)
      call hbook1(826,'SSD 1A',4096,1.,4097.,0)
      call hbook1(827,'SSD 1B',4096,1.,4097.,0)
      call hbook1(828,'SSD 1C',4096,1.,4097.,0)
      call hbook1(829,'SSD 1D',4096,1.,4097.,0)
      call hbook1(830,'SSD 1E',4096,1.,4097.,0)
      call hbook1(831,'SSD 1F',4096,1.,4097.,0)


      
c      call hbook2(412,'H3vsE 1', 100,0.,20.,100,0.,20.,0)
c      call hbook2(413,'H3vsE 2', 100,0.,20.,100,0.,20.,0)
c      call hbook2(414,'H3vsE 3', 100,0.,20.,100,0.,20.,0)
c      call hbook2(415,'H3vsE 4', 100,0.,20.,100,0.,20.,0)
c      call hbook2(428,'ExvSSDa t', 200,-2.,38.,100,6.,26.,0)
c      call hbook2(429,'ExvSSDp t', 200,-2.,38.,100,6.,26.,0)

c      call hbook2(400,'H3 1',100,0.,20.,150,0.,550.,0)
c      call hbook2(401,'H3 2',100,0.,20.,150,0.,550.,0)
c      call hbook2(402,'H3 3',100,0.,20.,150,0.,550.,0)
c      call hbook2(403,'H3 4',100,0.,20.,150,0.,550.,0)

c      call hbook2(404,'H3 B',100,0.,20.,150,0.,550.,0)
c      call hbook2(405,'H5',100,0.,20.,150,0.,550.,0)
c      call hbook2(406,'R',100,0.,20.,150,0.,550.,0)
c      call hbook2(407,'H3vsH5',100,0.,20.,100,0.,20.,0)

c      call hbook2(408,'H5vsR 1', 100,0.,20.,100,0.,20.,0)
c      call hbook2(409,'H5vsR 2', 100,0.,20.,100,0.,20.,0)
c      call hbook2(410,'H5vsR 3', 100,0.,20.,100,0.,20.,0)
c      call hbook2(411,'H5vsR 4', 100,0.,20.,100,0.,20.,0)


c      call hbook2(416,'ExvSSD1 t', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(417,'ExvSSD2 t', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(418,'ExvSSD3 t', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(419,'ExvSSD4 t', 200,-2.,38.,100,0.,20.,0)

c      call hbook2(420,'ExvSSD1 b', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(421,'ExvSSD2 b', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(422,'ExvSSD3 b', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(423,'ExvSSD4 b', 200,-2.,38.,100,0.,20.,0)

c      call hbook2(424,'ExvSSDa t', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(425,'ExvSSDp t', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(426,'ExvSSDd t', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(427,'ExvSSDt t', 200,-2.,38.,100,0.,20.,0)


c      call hbook2(432,'ExvSSDa b', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(433,'ExvSSDp b', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(434,'ExvSSDd b', 200,-2.,38.,100,0.,20.,0)
c      call hbook2(435,'ExvSSDt b', 200,-2.,38.,100,0.,20.,0)

c*** Ntuple
      call hbooknc(700,'XYAB',8,'Block1',xyab,chtags)

      return  

      end
