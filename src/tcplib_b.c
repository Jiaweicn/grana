/*
	tcp_lib.c ... DAQ TCP library
  Copyright (C) 1995  A. Tamii
  Author:   A. Tamii
	Facility: Department of Physics, Kyoto University 
	          & Research Center for Nuclear Physics
  Created:  08-MAY-1995 by A. Tamii
	          20-JAN-1997 by A. Tamii (Bug Fix)
	          19-JUL-1997 by A. Tamii (Bug Fix)
                  02-APL-2000 by T. Kawabata (modified for tuba)
*/

#include <stdio.h>
#include <stdlib.h>
#ifdef OSF1
#include <strings.h>
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#ifdef Linux
#include <unistd.h>
#endif

#if 0
#include "router.h"
#endif

/*
	router.h ... Definitions for Router (Global Use)
  Copyright (C) 1995  A. Tamii
  Author:   A. Tamii
	Facility: Department of Physics, Kyoto University 
	          & Research Center for Nuclear Physics
  Created:  24-JAN-1995 by A. Tamii (for Solaris 2.3)
  Version 2.0  20-JAN-1997 by A. Tamii (Change long to int for 64bit Deg Unix)
*/

#include <sys/ipc.h>
#include <sys/msg.h>

/*** constant definitions ***/
#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

#ifndef O_SAMPLE
#define O_ALL    0x00              /* transfer all blocks */
#define O_SAMPLE 0x08              /* sampling connection */
/* must be consistent with the value defined in rfmtypes.h */
#endif

#define ROUTER_DAQDATA_PORT  1126   /* default daqdata port */
#define RTR_NAME_LEN         31

/*** message buffer block ***/
typedef struct rtr_msg{
#ifdef OSF1
  mtyp_t   mtype;        /* mtype=target pid, if mtype==1, target=router */
#else
  long     mtype;        /* mtype=target pid, if mtype==1, target=router */
#endif
  long     pid;          /* sender process ID */
  long     cmd;          /* command to router or reply from router */
  long     arg[3];       /* optional arguments */
  char     name[RTR_NAME_LEN+1]; /* message sender/target name */
  long     reserved[18];
} rtr_msg_t;

/*** first information via TCP/IP ***/
typedef struct rtr_inet_info1{
  int      ver;          /* version = (1.0 or 2.0)* 256 */
  short    unit;         /* rfm unit  (0 for ver 2.0)*/
  short    stream;       /* rfm stream number (0 for ver 2.0)*/
  int      mode;         /* open mode */
  char     router[4];    /* name of the router (when stream=0) */
  char     user[16];     /* user name */
  char     program[16];  /* program name */
  char     uname[16];    /* user name on the host*/
} rtr_inet_info1_t;

typedef struct rtr_inet_info2{
  int      result;       /* result */
  int      blk_size;     /* 1 block size (fixed) */
  int      res1;         /* reserved */
  int      res2;         /* reserved */
} rtr_inet_info2_t;

typedef struct rtr_inet_info3{
  int      blk_count;    /* total count of the sent blocks */
  int      blk_size;     /* 1 block size (fixed) */
  int      res[14];      /* reserved */
} rtr_inet_info3_t;

/*** router messages ***/
#define RTR_CONNECT    1     /* Request Connection to Router */
#define RTR_DISCONNECT 2     /* Request Disconnection to Router */
#define RTR_REFUSE     3     /* Refuse connection */
#define RTR_BUFFER     4     /* Send Buffer Information to Consumer */
#define RTR_RELEASE    5     /* Release Buffer */
#define RTR_GETBUF     6     /* Request Buffer to Write */
#define RTR_SUSPEND    7     /* Suspend to Give Buffer */
#define RTR_STATUS     50    /* Show Status */
#define RTR_TERMINATE  100   /* Terminate Router Process */

#define ROUTER_VERSION     ((3<<8)|0)  /* Router version */
#define ROUTER_VERSION_MIN ((3<<8)|0)  /* Minimum client version */

/*** TCP/IP Message ***/
#define E_NOERR        0     /* No error */
#define E_VERSION      1     /* Version is too high */
#define E_INVAL        2     /* Invalid Data */
#define E_NODEV        3     /* No such router */
#define E_NOMEM        4     /* No more memory is available */

#define ROUTER_INET_VERSION_1  (1<<8)
#define ROUTER_INET_VERSION_2  (2<<8)

#define ROUTER_DAQDATA_SERVICE "daqdata"
#define ROUTER_DAQMSG_SERVICE  "daqmsg"

/*** message types ***/
#define RTR_MTYPE_ROUTER  1  /* target process = router */

#define remove_item(item) { \
    (item)->prev->next = (item)->next; \
    (item)->next->prev = (item)->prev; \
}

#define insert_item(place,item) { \
        (item)->prev = (place)->prev; \
        (item)->next = (place); \
	(item)->prev->next = (item); \
	(item)->next->prev = (item); \
}

/*** library functions ***/
key_t get_key(char*, uid_t uid);
void *get_shmbuf(int);
int router_connect(char *router, uid_t uid, char *name, int mode);
void router_disconnect(int fd);
int router_get_buf(int fd, void *buf, long *size);
int router_request_buf(int fd, long size);
int router_release_buf(int fd, void *buf, long);

/*****************************/
static int  sd;
static int  bufsize;


/*--- show error messages ---*/
#if 0
static int tcp_showerr_info2(int i)
#endif
static int tcp_showerr_info2(int i)
{
  switch(i){
  case E_NOERR:
    fprintf(stderr, "stdlib: No error.\n");
    break;
  case E_VERSION:
    fprintf(stderr, "stdlib: Client version is too high.\n");
    break;
  case E_INVAL:
    fprintf(stderr, "stdlib: Invalid connection information.\n");
    break;
  case E_NODEV:
    fprintf(stderr, "stdlib: No router process to connect.\n");
    break;
  default:
    fprintf(stderr, "stdlib: Unkown error.\n");
    break;
  }
  return 0;
}

/*--- open tcp connection ---*/
int tcp_open(host, port, stream, mode, proc_name, buf_size)
		 char *host;       /* server host name (DNS name or IP Address) */
		 int  port;        /* port number (if port==0, default port is used) */
		 int  stream;      /* stream number on RFM (default=1) */
		 int  mode;        /* O_ALL (=0x00) or O_SAMPLE (=0x08) */
		 int  *buf_size;   /* buffer size (returned from this procedure) */
		 char *proc_name;  /* client program name */
{
	struct sockaddr_in name;
  struct servent *serv_p;
	struct hostent *hp;
  rtr_inet_info1_t  info1;
	rtr_inet_info2_t  info2;
	char   *buf;
  int    i;
  int    count, n;
  char   *ptr;
  char   str[256];

	/* get services port */
	if(port==0){
		if((serv_p=getservbyname(ROUTER_DAQDATA_SERVICE, "tcp"))==NULL){
			fprintf(stderr, "tcplib: Couldn't find service %s.\n",
							ROUTER_DAQDATA_SERVICE);
			fprintf(stderr, "tcplib: Trying the default port (%d).\n",
							ROUTER_DAQDATA_PORT);
			port = ROUTER_DAQDATA_PORT;
		}else{
			port = serv_p->s_port;
		}
	}
	name.sin_port = htons(port);

	/* get server IP address */
	name.sin_family = AF_INET;
	if((hp=gethostbyname(host))==NULL){
		fprintf(stderr, "tcplib: Couldn't find the host %s.\n", host);
		return -1;
	}
	strncpy((char*)&name.sin_addr.s_addr, hp->h_addr, hp->h_length);

	/* connect to server */
	for(i=0; i<5; i++){
		fprintf(stderr, "tcplib: Trying to connect to %s at port %d...\n",
					host, ntohs(name.sin_port));
		/* open tcp socket */
		if((sd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
			perror("tcplib: socket");
			return -1;
		}
		if(connect(sd, (struct sockaddr*)&name, sizeof(name))==0)
			break;
		perror("tcplib: connect");
		name.sin_port = htons(ntohs(name.sin_port)+1);
		close(sd);
	}
	if(i>=5){
		fprintf(stderr, "Couldn't connect to the host.\n");
		return -1;
	}
	fprintf(stderr, "tcplib: Connected successfully.\n");

	/* prepare information to the server */
	strncpy(info1.program, proc_name, sizeof(info1.program));
	info1.unit   = htons(0);
	info1.stream = htons(stream);
  info1.mode = htonl(mode);
	info1.ver = htonl(ROUTER_INET_VERSION_1);
/*	info1.res1 = htonl(0); */
  cuserid(str);
  if(cuserid(str)==(int *)NULL)
		strncpy(info1.user, "Anonymous", sizeof(info1.user));
	else
		strncpy(info1.user, str, sizeof(info1.user));
/*  strcpy(info1.res2, ""); */

	/* send information to the server */
	if(write(sd, &info1, sizeof(info1))<0){
		perror("tcplib: write");
		close(sd);
	}
	
	/* wait for response */
  count = 0;
  ptr  = (char*)&info2;
	while(count<sizeof(info2)){
		if((n=read(sd, ptr, sizeof(info2)-count))<0){  /* wait for the reply */
			perror("tcplib: read");
			return -1;
		}
		if(n==0){
			fprintf(stderr, "tcplib: Connection was closed from the server.\n");
			return -1;
		}
		count += n;
	}
	
	/* check the reply from the server */
	i = ntohl(info2.result);
	if(i!=E_NOERR){
		fprintf(stderr, "tcplib: Error Reply from the server (%d).\n", i);
		tcp_showerr_info2(i);
		close(sd);
		return -1;
	}

	/* set block size */
	*buf_size = bufsize = ntohl(info2.blk_size);
	return 0;
}

/*--- get one block ---*/
int tcp_get_buf(buf)
	char *buf;
{
	int  count, size;	
  int    reply;

	/* get a block from the server */
	count = 0;
	while(1){
		if((size = read(sd, &buf[count], bufsize-count))<0){
			perror("tcplib: read");
			return -1;
		} else if(size==0) break;
		count += size;
		if(count>=bufsize) break;
	}
	if(count==0) return 1;

	/* send respose to the server */
	reply = 0;
	if(write(sd, &reply, sizeof(int))<0){
		perror("tcplib: write");
		return(-1);
	}

  return 0;
}

/*--- close tcp connetion ---*/
int tcp_close(){
	fprintf(stderr, "tcplib: Close the connection.\n");
	close(sd);
	return 0;
}

/******--- Fortran Interfaces ---*****/
int tcp_open_(host, port, stream, mode, proc_name, buf_size)
		 char *host;       /* server host name (DNS name or IP Address) */
		 int  *port;       /* port number (if port==0, default port is used) */
		 int  *stream;     /* stream number on RFM (default=1) */
		 int  *mode;       /* O_ALL (=0x00) or O_SAMPLE (=0x08) */
		 int  *buf_size;   /* buffer size (returned from this procedure) */
		 char *proc_name;  /* client program name */
{
	return tcp_open(host, *port, *stream, *mode, proc_name, buf_size);
}

int tcp_get_buf_(buf)
	char *buf;
{
	return tcp_get_buf(buf);
}

int tcp_close_(){
	return tcp_close();
}

/*****--- Old Fortran Interfaces ---******/
int tcpopen_(host, stream, mode, buf_size)
		 char  *host;
		 int   *stream;
		 int   *mode;
		 int  *buf_size;
{
	return tcp_open(host, 0, *stream, *mode, "TCPLIB_FORT", buf_size);
}

int tcpgtbuf_(buf)
	char *buf;
{
	return tcp_get_buf(buf);
}

int tcpclose_(){
	return tcp_close();
}
