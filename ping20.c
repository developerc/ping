#ifdef VMS
#include <descrip.h>        /* VMS descriptor stuff */
#include <errno.h>          /* Unix style error codes for IO routines. */
#include <in.h>             /* internet system Constants and structures. */
#include <inet.h>           /* Network address info. */
#include <iodef.h>          /* I/O FUNCTION CODE DEFS */
#include <lib$routines.h>   /* LIB$ RTL-routine signatures. */
#include <netdb.h>          /* Network database library info. */
#include <signal.h>         /* UNIX style Signal Value Definitions */
#include <socket.h>         /* TCP/IP socket definitions. */
#include <ssdef.h>          /* SS$_<xyz> sys ser return statistics */
#include <starlet.h>        /* Sys ser calls */
#include <stdio.h>          /* UNIX ’Standard I/O’ Definitions   */
#include <stdlib.h>         /* General Utilities */
#include <string.h>         /* String handling function definitions */
#include <ucx$inetdef.h>    /* UCX network definitions */
#include <unixio.h>         /* Prototypes for UNIX emulation functions */
#else
	#include <errno.h>
	#include <sys/types.h>
	#include <stdio.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <sys/uio.h>
	#endif
//set proc/priv=all
//set all privileges

typedef struct tagICMPHDR {
	u_char Type;
	u_char Code;
	u_short Checksum;
	u_short ID;
	u_short Seq;
	char Data;
} ICMPHDR, *PICMPHDR;

typedef struct tagECHOREQUEST {
	ICMPHDR icmpHdr;
	unsigned long int dwTime;
	char cData[64];
} ECHOREQUEST, *PECHOREQUEST;

typedef struct tagIPHDR
{
	u_char  VIHL;			
	u_char	TOS;			
	short	TotLen;			
	short	ID;			
	short	FlagOff;		
	u_char	TTL;			
	u_char	Protocol;		
	u_short	Checksum;		
	struct	in_addr iaSrc;
	struct	in_addr iaDst;	
}IPHDR, *PIPHDR;

typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;	

int DoPingOperation();
u_short CheckSum(u_short *addr, int len);

void main(int argc, char **argv){
//      printf("%s", argv[2]);

	DoPingOperation();
}	

int DoPingOperation(){
	int rawSocket;
	char *ash = "127.0.0.1";
    struct in_addr in;
    struct hostent *hp;
	struct sockaddr_in sDest;
	struct sockaddr_in sSrc;
	int iRet;
	
	//creating socket, getting his descriptor
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(rawSocket == -1){
		printf("rawSocket = -1\n");
		return 0;
	}

	//getting pointer to hostent of IP-address 127.0.0.1 (my own)
    if( inet_aton( ash, &in ))
        if(hp = gethostbyaddr((char*)&in.s_addr,sizeof(in.s_addr),AF_INET))
                        printf( "%s\n", hp->h_name );
                else
                        printf( "gethostbyaddr error: %d\n", h_errno );
    else
        printf( "inet_aton error\n" );
	
		sDest.sin_addr.s_addr = inet_addr(ash);
        sDest.sin_family = AF_INET;
        sDest.sin_port = 0;
        printf("Pinging 127.0.0.1 [%s]\n", inet_ntoa(sDest.sin_addr));

		//sending echo-request
	static ECHOREQUEST echoReq;
	echoReq.icmpHdr.Type = 8;
	echoReq.icmpHdr.Code = 0;
	echoReq.icmpHdr.ID = 0;
	echoReq.icmpHdr.Seq = 0;
	memset(echoReq.cData, 'E', 64);
	echoReq.icmpHdr.Checksum = CheckSum((u_short *)&echoReq, sizeof(ECHOREQUEST));
	iRet = sendto(rawSocket, (char *)&echoReq, sizeof(ECHOREQUEST), 0, 
		(struct sockaddr *)&sDest, sizeof(struct sockaddr_in));
	printf("Sended %d bytes\n", iRet);
		
		//getting reply
	ECHOREPLY echoReply;
	unsigned int nAddrLen = sizeof(struct sockaddr_in);
	iRet = recvfrom(rawSocket, (char *)&echoReply, sizeof(ECHOREPLY), 0, (struct sockaddr *)&sSrc, &nAddrLen);
	if (iRet < 0) {
		printf("Error by receiving of reply\n");
	} else {
		printf("Pinging successful!!\n");
	}
	
	close (rawSocket);
	return 1;
}

unsigned short CheckSum(unsigned short *addr, int len)
{
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;

    while (nleft > 1)
    {
      sum += *w++;  // 
      nleft -= 2;
    }
    if (nleft)
    {
      *(u_char *)(&answer) = *(u_char *)w;  // 
      sum += answer;
    }
     //add back carry outs from top 16 bits to low 16 bits
    sum = (sum >> 16) + (sum & 0xffff);   // 
    sum += (sum >> 16);   // carry bit
    answer = ~sum;    
    return (answer);
}
