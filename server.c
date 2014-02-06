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

    int servSock;				/* Server Socket */
    int clntSock;				/* Client Socket */
    struct sockaddr_in servAddr;		/* Local address */
    struct sockaddr_in clntAddr;		/* Client address */
    unsigned short servPort;		/* Server port */
    unsigned int clntLen;			/* Length of address data struct */
    // character array for send and recieve
    // you are going to recv check what asking for 
    // fill out another buffer for sending stuffs
    char nameBuf[BUFSIZE];			/* Buff to store account name from client */
    int balance;				/* Place to record account balance result */

    char sndBuf[SNDBUFSIZE];        /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];        /* Receive Buffer */

    int countSaving = 0;
    int countChecking = 0;
    int countRetirement = 0;
    int countCollege = 0;
    
    // check valid arguments
    if (argc != 2){
       printf("Incorrect number of arguments. The correct format is:\n\tserverPort");
       exit(1);
    }
    
    // convert string to integer needs to be unsigned short
    servPort = (unsigned short)(atoi(argv[1]));
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
        //DieWithSystemMessage("socket() failed");
        // perror() -> posted straight to command prompt; not buffered
        perror("socket() failed");
        //close(servSock);
        exit(EXIT_FAILURE);
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
    // at location of servAddr, it will make 0 for size of it
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    // bind listening socket to the special IP address INADDR_ANY
    // when receiving, a socket bound to this address receives packets from all interface
    // let the system pick up the address running on, wildcard address INADDR_ANY

    // INADDR_ANY -> IP address of server (because server doesn't matter what IP address)
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // first arg is name of file running

    // host to network
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

    // bind address and port because the socket doesn't know yet; socket only knew type of connection
    if(bind(servSock,(struct sockaddr*) &servAddr, sizeof(servAddr))<0){
        // DieWithSystemMessage("bind() failed");
        perror("bind() failed");
        close(servSock);
        exit(EXIT_FAILURE);
    }





    /* Listen for incoming connections */
    // allow incoming connections from clients
    // only necessary for the stream-oriented (connection oriented) data modes
    // i.e. socket types SOCK_STREAM, SOCK_SEQPACKET
    // int listen(int sockfd, int backlog);
            // sockfd   valid socket descriptor
            // backlog  integer representing the number of pending connections that can be queued up at any one time
    
    // starts actual socket
    if(listen(servSock, MAXPENDING)<0){
        // DieWithSystemMessage("listen() failed");
        // perror prints my message then errno message
        perror("listen() failed");
        close(servSock);
        exit(EXIT_FAILURE);
    }




    /* Loop server forever*/
    while(1){

        memset(&sndBuf, 0, SNDBUFSIZE);
        memset(&rcvBuf, 0, RCVBUFSIZE);

	/* Accept incoming connection */
        // struct sockaddr_in clntAddr;
        // set length of client address structure

        unsigned int clntAddrLen = sizeof(clntAddr);

        // now wait for a client to connect

        // waits on accepts; accept pauses program until someone connects

        // outgoing socket connecting you to a specific client so you can send things through the socket
        // also fills clientAddress struct

        // accept gives you a socket to return and it saves the incoming address
        // clntAddr-> address of the client connecting
        int clntSock = accept(servSock, (struct sockaddr *) & clntAddr, &clntAddrLen);
        if(clntSock<0){
            // perror("print this"); -> global variable in errno; functions if error set errno
            // errno is a number with a defined message
            // if error close(socket)
            // exit(EXIT_FAILURE) -> returns from program
            // DieWithSystemMessage("accept() failed");
            perror("Client socket() failed... Closing Server");
            close(servSock);
            exit(EXIT_FAILURE);
        }
    

    // ~~~~ clntSock is now connected to a client!



    // recv & send take flag 0

    // TODO decide what you are sending TEXT OR INT NEEDS TO BE CONSISTENT ON BOTH SIDES
    // sprintf -> instead of printing to console // TODO FIND TAKES BUFFER W/O size
        //snprintf -> accepts buffer lenBuffer, format string & params


    // send full text BAL: MY CHECKING
    // send binary encoding
    // this is just in array anything in here is send
    // void * -> pointer to anything


	/* Extract the account name from the packet, store in nameBuf */

        // ssize_t recv(int s, void *buf, size_t len, int flags);
        // recv returns number of bytes actually received or -1 for error & sets errno
        // recv returns 0 if remote size closed connection
        // QUESTION: WHY DO I NEED THE RECV SIZE
        // rcvBuf is already a pointer because... char array == char pointer
        // TODO sending/rcving server or client QUESTION
        int recvSize = recv(clntSock, rcvBuf, RCVBUFSIZE, 0);
        if(clntSock==-1){
            perror("recv() failed.. Closing server... Nothing to do.");
            close(servSock);
            exit(EXIT_FAILURE);
        }
        // send&recv check packetSize things
        // recv() give buffer length 
        // pass in array in length function can't tell size of

    /* Look up account balance, store in balance */
        // lookup stored stuff

        // strcmp return 0 if same value; 
        if(strcmp("BAL mySavings", rcvBuf)==0){
            countSaving++;
            balance = 1000;
            snprintf(sndBuf, SNDBUFSIZE, "mySavings BALANCE  %d", balance);

        }
        else if(strcmp("BAL myChecking", rcvBuf)==0){
            countChecking++;
            balance = 2000;
            snprintf(sndBuf, SNDBUFSIZE, "myChecking BALANCE  %d", balance);
        }        
        else if(strcmp("BAL myRetirement", rcvBuf)==0){
            countRetirement++;
            balance = 3000;
            snprintf(sndBuf, SNDBUFSIZE, "myRetirement BALANCE  %d", balance);
        }        
        else if(strcmp("BAL myCollege", rcvBuf)==0){
            countCollege++;
            balance = 4000;
            snprintf(sndBuf, SNDBUFSIZE, "myCollege BALANCE  %d", balance);
        }


        else if(strcmp("COUNT mySavings", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "mySavings COUNT  %d", countSaving);
        }
        else if(strcmp("COUNT myChecking", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myChecking COUNT  %d", countChecking);
        }        
        else if(strcmp("COUNT myRetirement", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myRetirement COUNT  %d", countRetirement);
        }        
        else if(strcmp("COUNT myCollege", rcvBuf)==0){
            snprintf(sndBuf, SNDBUFSIZE, "myCollege COUNT  %d", countCollege);
        }
        // else is a BAD COMMAND
        else{
            // perror() -> posted straight to command prompt; not buffered
            snprintf(sndBuf, SNDBUFSIZE, "YOU FAILED TO GIVE CORRECT PROMPT. Please try again.");
        }

	/* Return account balance to client */
	/*	FILL IN	    */
        // send()
        
        // char * name -> making it a pointer
        // if you have a pointer and put a star in front of it, dereference
        // & gets you the address of a thing
        // TODO TRY TO USE SENND BUFFER
        int sentSize = send(clntSock, sndBuf, SNDBUFSIZE, 0);
        // at the end of while loop need to close client connection socket
        // then loops back and make a new one

        // TODO LOOKUP SHUTDOWN
        close(clntSock);
    }

    return 0;
}

