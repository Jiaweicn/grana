#Makefile for ana_v1.64
#05/07/18

#####################################################
# architechture
##################################
# miho
####
#ARCH = -D_DEC
#CERNDIR =/usr/local/cern/97a
#FC	= f77
#FLIB    = -lfor -lots 
#TCPLIB  = tcplib.o
##################################

# saho (AIX)
####
#ARCH = -D_AIX
#CERNDIR =/usr/local/cern/2000
#FC	= xlf -qextname
#FLIB    = -lxlf90
#TCPLIB  = tcplib.o
#CFLAGS =
#FFLAGS =
##################################

# hozumi (Linux)
####
#ARCH = -D_LINUX
#CERNDIR =/usr/local/cern/2006b
#CERNDIR =/home/e308/lib/cern/2005
#FC	= gfortran
#FLIB    = 
TCPLIB  = tcplib.o
#CFLAGS =
#FFLAGS =
##################################


##################################
# linux (horn)
####
#ARCH = -D_LINUX
#CERNDIR =/horn/cern/98
#FC	= g77
#CC      = cc -O2 
#FLIB    = -lf2c
#TCPLIB  = tcplib.o
##################################

##################################
# linux (tuba)
####
#ARCH = -D_LINUX
#CERNDIR =/cern/2000
#FC	= g77
#CC      = cc -O
#FLIB    = -lg2c -lnsl
#TCPLIB  = tcplib_b.o
##################################

#############
# COMMON
INCDIR=../include
DEPEND.INC=.depend


#############
# OPTIONS 
#do not edit 
#GLFLAG = -D_LAS_OFF
#GLFLAG = -D_GR_OFF -D_LAS_VDC_XU
#GLFLAG = -D_GR_OFF
GLFLAG =
OPTIONS = -D_KEEP_BLOCK_SIZE $(ARCH)

#edit according to the situation
#VDCFLAG = -D_VDC_LEASTSQRT
#VDCFLAG = -D_VDC_MULT_HIT -D_ELIM_GAMMA -D_VDC_TWOWIRE_ON -D_VDC_CLUSTER_CORR \
#          -D_VDC_INTERPOLATION
VDCFLAG = -D_VDC_MULT_HIT -D_ELIM_GAMMA -D_VDC_INTERPOLATION \
          -D_VDC_LEASTSQRT
#VDCFLAG = -D_VDC_MULT_HIT -D_ELIM_GAMMA -D_VDC_CLUSTER_CORR \
#          -D_VDC_INTERPOLATION -D_VDC_LEASTSQRT
#VDCFLAG = -D_ELIM_GAMMA -D_VDC_TWOWIRE_ON -D_VDC_CLUSTER_CORR
VDCEFFLAG = -D_VDC_EFF_CHECK
FPPFLAG = -D_FPP_ON -D_MWPC_EFF_CHECK
#FPPFLAG = -D_VER_POL -D_FPP_ON -D_MWPC_EFF_CHECK


#LINK    = cc -lgfortran
LINK    = gfortran
CFLAG = -O2 -Wall -I$(INCDIR)
CC = gcc $(CFLAG)
CUR_ANA_VER = 1.70

########
# EXTERNAL LIB
CERNLIB = -L/opt/root-6.10.08/install/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic -pthread -std=c++11 -m64 -I/opt/root-6.10.08/install/include
EXTLIB = $(CERNLIB) $(FLIB) -lm

########
# COMMON LIB
#ANALIB = analib7.o histdef.o
EVELIB  = eventid.o fera.o mwpcpcos2.o startup.o vdccalib.o blp.o \
	go_down.o vectcalc.o raytrace.o scaler.o ssd_3351.o kinelib.o \
	vdc3377_v6.o
COMLIB =  $(TCPLIB) $(ANALIB) $(EVELIB)

########
# GR LIB
GRVDCLIB = vdceffv2.o
GRPCLIB  = mwpceff.o
GRMATLIB = grmat.o
GRLIB    = $(GRVDCLIB) $(GRPCLIB) $(GRMATLIB)
GROPT    = $(FPPFLAG) $(VDCEFFLAG)


########
# LAS LIB
LASMATLIB = lasmat.o
LASLIB = $(LASMATLIB)
LASOPT = 


############# 
# MAIN


anagr: ana_v$(CUR_ANA_VER).c $(COMLIB) $(GRLIB) $(LASLIB)  
	g++ $(CFLAG) -o $@ ana_v$(CUR_ANA_VER).c \
	$(GRLIB) $(LASLIB) $(COMLIB) \
	$(GROPT) $(LASOPT) $(OPTIONS) $(EXTLIB) 


ana_v$(CUR_ANA_VER): ana_v$(CUR_ANA_VER).c $(COMLIB) $(GRLIB) $(LASLIB)
	$(CC) -o ana $@.c $(GRLIB) $(LASLIB) $(COMLIB) \
	$(GROPT) $(LASOPT) $(OPTIONS) $(EXTLIB)


c.o.:
	$(CC) -c $<


vdc3377_v6.o: vdc3377_v6.c
	$(CC) $(VDCFLAG) $(GLFLAG) $(VDCEFFLAG) \
              -c -o vdc3377_v6.o vdc3377_v6.c

vdceffv2.o: vdceffv2.c 
	$(CC) -c -o vdceffv2.o vdceffv2.c $(VDCEFFLAG) 


############
# SCALER
anasc: anasc.c $(COMLIB)
	$(CC) -o anasc anasc.c	$(COMLIB) $(EXTLIB) 

clean:
	/bin/rm -f *.o ana anagr analas anap2p ananpol anasc core 

depend:
	@rm -f $(DEPEND.INC)
	@touch $(DEPEND.INC)
	-@makedepend -- -I$(INCDIR) -- anasc.c \
	$(COMLIB:%.o=%.c) $(GRPCLIB:%.o=%.c) $(LASMATLIB:%.o=%.c) \
	$(GRMATLIB:%.o=%.c) \
	-o .o -a -f $(DEPEND.INC) 2> /dev/null


	-@makedepend -- -I$(INCDIR) -- $(GRVDCLIB:%.o=%.c) $(VDCFLAG) \
	$(GRFLAG) $(VDCEFFLAG)  -o .o -a -f $(DEPEND.INC) 2> /dev/null

	-@makedepend -- -I$(INCDIR) -- $(LASVDCLIB:%.o=%.c) $(VDCFLAG) \
	$(LASFLAG)  -o .o -a -f $(DEPEND.INC) 2> /dev/null

	-@makedepend -- -I$(INCDIR) -- \
	ana_v$(CUR_ANA_VER).c $(GROPT) -o.o -a -f $(DEPEND.INC) 2> /dev/null


-include $(DEPEND.INC)

