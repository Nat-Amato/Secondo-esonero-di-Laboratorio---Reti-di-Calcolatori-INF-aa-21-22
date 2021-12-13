/*********************************************************************************
 *  Title: Lab - Reti di Calcolatori (INF) aa 21-22                              *
 *  Year:  2021                                                                  *
 *  Description: UDP+DNS client/server application,                              *
 *  			 where the server is a remote calculator                         *
 *  			 that responds to client requests                                *
 *  Author:	Natale Amato			                                             *
 *                                                                               *
 *********************************************************************************/
#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h> /* for memset() */
#define ECHOMAX 255
#define DEFAULTNAME "localhost:60000"
void ErrorHandler(char *errorMessage) {
	printf(errorMessage);
}
void ClearWinSock() {
#if defined WIN32
	WSACleanup();
#endif
}

#endif /* PROTOCOL_H_ */
