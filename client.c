    /*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Xialin Yan
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura 
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define REPLYLEN 32

/* The main function */
int main(int argc, char *argv[])
{

    int sock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* server address structure */

    char *accountName;		    /* Account Name  */
    char *servIP;		    /* Server IP address  */
    unsigned short servPort;	    /* Server Port number */
    
    char *balCount;

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int balance;		    /* Account balance */

    /* Get the Account Name from the command line */
    if (argc != 4){
	   printf("Incorrect number of arguments. The correct format is:\n\t BAL/COUNT + accountName + serverIP + serverPort");
	   exit(1);
    }
    balCount = argv[1];
    accountName = argv[2];
    servIP = argv[3];
    servPort = atoi(argv[4]);
    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Get the addditional parameters from the command line */
    /*	    FILL IN	*/




    /* Create a new TCP socket*/
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock<0){
        // DieWithSystemMessage("socket() failed");
        perror("socket() failed");
        // close(servSock);
        exit(EXIT_FAILURE);
    }



    /* Construct the server address structure */
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    // convert address
    // inet_pton - convert IPv4 and IPv6 addresses from text to binary form

    // inet_pton sets servAddr.sin_addr.s_addr part of struct based on servIP
    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
    if(rtnVal == 0){
        DieWithSystemMessage("inet_pton() failed", "invalid address string");
    }
    else if(rtnVal < 0){
        DieWithSystemMessage("inet_pton() failed");
    }
    servAddr.sin_port = htons(servPort);        // server port



    /* Establish connecction to the server */
    if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
        // DieWithSystemMessage("connect() failed");
        perror("connect() failed");
        close(sock);
        exit(EXIT_FAILURE);

    }
     // determine input length
    // echoString TODO send buffer instead of echoString
    // TODO fill send buffer with commands snprintf()
    // connected now you can send

    // TODO NEEDS TO PUT THINGS IN SENDBUFFER
    
    /* Send the string to the server */
    snprintf(sndBuf, SNDBUFSIZE, "%s %s", balCount, accountName);

    ssize_t numBytes = send(sock, sndBuf, SNDBUFSIZE, 0);
    if(numBytes < 0){
        // DieWithSystemMessage("send() failed");
        perror("send() failed... TRY AGAIN");
        close(sock);
        exit(EXIT_FAILURE);
    }
    // TODO ECHOLENGTH WHAT ERROR IS BUFFER SIZE THING NOT EQUAL TO NUMBYTES

    /* Receive and print response from the server */

    int recvSize = recv(sock, rcvBuf, RCVBUFSIZE, 0);
    if(recvSize==-1){
        perror("recv() error");
        exit(1);
    }

    // TODO HOW TO DO BUFFER THINGS
    // printf("client: received '%s'\n",rcvBuf);

    
    // unsigned int totalBytesRcvd = 0;    // count of total bytes received
    


    // balCount = argv[1];
    // accountName = argv[2];
    // servIP = argv[3];
    // servPort = atoi(argv[4]);

    // *rcvBuf dereferences it and it makes it gives first element
    printf("%s\n", rcvBuf);

    close(sock);
    
    return 0;
}



// putty 
// win scp
// windows is not posix
// cygwin gives you posix on windows
