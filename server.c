/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Xialin Yan
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura 
* DESCRIPTION:	Network Server Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

static const int MAXPENDING = 5;    // Maximum outstanding connection requests

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort;		/* Server port */
    unsigned int clntLen;			/* Length of address data struct */

    char nameBuf[BUFSIZE];			/* Buff to store account name from client */
    int  balance;				/* Place to record account balance result */


    /* Create new TCP Socket for incoming requests*/

    // socket takes 3 arguments: domain, type, protocol
    // domain: protocol family of the created socket
        // AF_INET for IPv4
        // AP_INET6 for IPv6
        // AP_UNIX for local socket
    // type:
        // SOCK_STREAM stream-oriented
        // SOCK_DGRAM datagram
        // SOCK_SEQPACKET reliable sequenced packet service
        // SOCK_RAW raw protocols atop the network layer
    // protocol
        // defined in <netinet/in.h>
        // IPPROTO_.. TCP/UDP/SCTP/DCCP

    // socket() will return -1 if an error occured
    // int socket(int domain, int type, int protocol);

    servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(servSock<0){
        DieWithSystemMessage("socket() failed");
    }



    /* Construct local address structure*/
    // on server, need to associate server socket with address & port number
    
    // struct sockaddr_in{
    //      short            sin_family;    // domain
    //      unsigned short   sin_port;      // htons(3490)
    //      struct in_addr   sin_addr;      // struct
    //      char             sin_zero[8];   // zero if want
    // };

    // struct in_addr{
    //      unsigned long s_addr;           // load with inet_pton()
    // };

    // inet_pton - convert IPv4 and IPv6 addresses from text to binary form
    struct sockaddr_in servAddr;
    // at location of servAddr, it will make 0 for size of it
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    // bind listening socket to the special IP address INADDR_ANY
    // when receiving, a socket bound to this address receives packets from all interface
    // let the system pick up the address running on, wildcard address INADDR_ANY
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);



    
    /* Bind to local address structure */
    // socket() -> only given protocol family but not assigned address
    // bind() -> assigns socket to an address
    // server socket needs to be associated with a local address and port
    // server specifies own address to bind (client supply server address to connect())
    // ^ need to agree to communicate; doesn't need client address
    
    // struct sockaddr{
    //      unsigned short      sa_family;          // address family
    //      char                sa_data[14];        // 14 byte of protocol address
    // };

    // bind() can fail if socket is already bound to the specified port
    // returns 0 if success and -1 if fail
    // int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
            // sockfd   represent socket to perform bind on
            // my_addr  pointer to sockaddr structur representing the address to bind to
            //addrlen   socklen_t size of sockaddr structure

    if(bind(servSock,(struct sockaddr*) &servAddr, sizeof(servAddr))<0){
        DieWithSystemMessage("bind() failed");
    }





    /* Listen for incoming connections */
    // allow incoming connections from clients
    // only necessary for the stream-oriented (connection oriented) data modes
    // i.e. socket types SOCK_STREAM, SOCK_SEQPACKET
    // int listen(int sockfd, int backlog);
            // sockfd   valid socket descriptor
            // backlog  integer representing the number of pending connections that can be queued up at any one time
    if(listen(servSock, MAXPENDING)<0){
        DieWithSystemMessage("listen() failed");
    }




    /* Loop server forever*/
    while(1){

	/* Accept incoming connection */
        struct sockaddr_in clntAddr;
        // set length of client address structure
        socketln_t clntAddrLen = sizeof(clntAddr);

        // now wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr *) & clntAddr, &clntAddrLen);
        if(clntSock<0){
            DieWithSystemMessage("accept() failed");
        }
        // clntSock is connected to a client!

	/* Extract the account name from the packet, store in nameBuf */
	/*  FILL IN     */

    /* Look up account balance, store in balance */
	/*	FILL IN	    */

	/* Return account balance to client */
	/*	FILL IN	    */

    }

}

