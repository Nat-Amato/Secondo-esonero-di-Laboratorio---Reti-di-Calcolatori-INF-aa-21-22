/*********************************************************************************
 *  Title: Lab - Reti di Calcolatori (INF) aa 21-22                              *
 *  Year:  2021                                                                  *
 *  Description: UDP+DNS client/server application,                              *
 *  			 where the server is a remote calculator                         *
 *  			 that responds to client requests                                *
 *  Author:	Natale Amato			                                             *
 *                                                                               *
 *********************************************************************************/
#include "protocol.h"

// Syntax check of the inserted operation
char verification(char input_string[]) {
	char bool = 't';
	char point = 'f';
	char space = 'f';
	if (strlen(input_string) < 5)
		bool = 'f';
	for (int i = 0; i < strlen(input_string) && bool == 't'; i++) {
		if ((i == 0)
				&& (input_string[i] != '+' && input_string[i] != '-'
						&& input_string[i] != '*' && input_string[i] != '/'))
			bool = 'f';
		if ((i == 1) && (input_string[i] != ' '))
			bool = 'f';
		if ((i > 1) && (input_string[i] < '0' || input_string[i] > '9'))
			bool = 'f';
		if ((i == 2) && (input_string[i] == '-')
				&& (input_string[i + 1] >= '0' && input_string[i + 1] <= '9'))
			bool = 't';
		if ((i > 2) && (input_string[i] == '.') && (point == 'f')) {
			point = 't';
			bool = 't';
		}
		if ((i > 2) && (input_string[i] == ' ') && (space == 'f')) {
			space = 't';
			point = 'f';
			bool = 't';
			if (input_string[i + 1] == '.' || (i == (strlen(input_string) - 1)))
				bool = 'f';
			if (input_string[i + 1] == '-')
				i++;
		}
		if ((i == (strlen(input_string) - 1))
				&& ((space == 'f') || input_string[i] == '-'))
			bool = 'f';
	}
	return bool;
}

int main() {
#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("error at WSASturtup\n");
		return EXIT_FAILURE;
	}
#endif
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	unsigned int fromSize;
	char echoString[ECHOMAX];
	char echoBuffer[ECHOMAX];
	int echoStringLen;
	int respStringLen;

// SOCKET CREATION
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		ErrorHandler("socket() failed");
		system("pause");
		exit(EXIT_FAILURE);
	}
// CONSTRUCTION OF THE SERVER ADDRESS
	int port;
	char name[50] = "\0";
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	do {
		printf("Enter name and port or press ENTER to default: ");
		char addr[500];
		fflush(stdin);
		gets(addr);
		if (strcmp(addr, "") == 0){
			strcpy(addr, DEFAULTNAME);
		}
		char *token = strtok(addr, ":"); // string separation by returning the DNS

// CONVERSION NAME TO ADDRESS
		struct hostent *host;
		strcpy(name, token);
		host = gethostbyname(token);
		if (host == NULL) {
			fprintf(stderr, "gethostbyname() failed.\n");
			system("pause");
			exit(EXIT_FAILURE);
		} else {
			struct in_addr *ina = (struct in_addr*) host->h_addr_list[0];
			char *ip = inet_ntoa(*ina);
			echoServAddr.sin_addr.s_addr = inet_addr(ip);
		}
		for (int i = 0; i == 0; i++) { // string splitting by returning the port
			token = strtok(NULL, ":");
			port = atoi(token);
			echoServAddr.sin_port = htons(port);
		}

		if (port < 49152 || port > 65535)
			printf("Bad port number %i, please insert a port number between 49152 and 65535 \n", port);
	} while (port < 49152 || port > 65535);

// INPUT ASSIGNMENT
	do {
		char bool;
		do {
			bool = 't';
			printf("Enter an operation or = to exit: ");
			gets(echoString); // String to send
			fflush(stdin);
			if ((echoStringLen = strlen(echoString)) > ECHOMAX) {
				ErrorHandler("echo word too long");
				system("pause");
				exit(EXIT_FAILURE);
			}
			if (strcmp(echoString, "=") != 0)	bool = verification(echoString);
			if (bool == 'f') printf("Invalid operation! Syntax example: + 23 45\n");
		} while (bool == 'f');
// SEND DATA TO THE SERVER
		if (sendto(sock, echoString, echoStringLen, 0,
				(struct sockaddr*) &echoServAddr, sizeof(echoServAddr))
				!= echoStringLen) {
			ErrorHandler("sendto() sent different number of bytes than expected");
			system("pause");
			exit(EXIT_FAILURE);
		}

// RECEIVE TOTAL TO THE SERVER
		if(echoString[0] != '='){
			fromSize = sizeof(fromAddr);
			respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*) &fromAddr, &fromSize);
			if (respStringLen == -1){
				fprintf(stderr, "Error: null reception.\n");
				system("pause");
				exit(EXIT_FAILURE);
			}
			if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
				fprintf(stderr, "Error: received a packet from unknown source.\n");
				system("pause");
				exit(EXIT_FAILURE);
			}
			echoBuffer[respStringLen] = '\0';
			char* ip = inet_ntoa(fromAddr.sin_addr);
			printf("Received result from server %s, ip %s: %s\n",name , ip, echoBuffer);
		}
	} while (echoString[0] != '=');

// CLOSING THE CONNECTION
	closesocket(sock);
	ClearWinSock();
	printf("\n"); // Print a final line feed
	system("pause");
	return EXIT_SUCCESS;
}
