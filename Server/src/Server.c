/*********************************************************************************
 *  Title: Lab - Reti di Calcolatori (INF) aa 21-22                              *
 *  Year:  2021                                                                  *
 *  Description: UDP+DNS client/server application,                              *
 *  			 where the server is a remote calculator                         *
 *  			 that responds to client requests                                *
 *  Author:	Francesco de Gioia											         *
 *                                                                               *
 *********************************************************************************/
#include "protocol.h"

float add(float a, float b) {
	return a + b;
}

float sub(float a, float b) {
	return a - b;
}

float mult(float a, float b) {
	return a * b;
}

float division(float a, float b) {
	return a / b;
}

// Extrapolates the numbers from the string coming from the client
void extrap(char output[], char input[], int n) {
	if (n == 1) {
		for (int i = 2; input[i] != ' '; i++)
			strncat(output, &input[i], 1);
	} else {
		int i;
		for (i = 2; input[i] != ' '; i++)
			;
		i++;
		for (; i < strlen(input); i++)
			strncat(output, &input[i], 1);
	}
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
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX] = "\0";
	// SOCKET CREATION
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		ErrorHandler("socket() failed");
		system("pause");
		exit(EXIT_FAILURE);
	}

// ASSIGNING A SOCKET ADDRESS
	int port;
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	do {
		printf("Enter name and port or press ENTER to default: ");
		char addr[500];
		fflush(stdin);
		gets(addr);
		if (strcmp(addr, "") == 0) {
			strcpy(addr, DEFAULTNAME);
		}
		char *token = strtok(addr, ":"); // string separation by returning the DNS

// CONVERSION NAME TO ADDRESS
		struct hostent *host;
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
			printf(
					"Bad port number %i, please insert a port number between 49152 and 65535 \n",
					port);
	} while (port < 49152 || port > 65535);

// BIND DELLA SOCKET
	if ((bind(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)))
			< 0) {
		ErrorHandler("bind() failed");
		system("pause");
		exit(EXIT_FAILURE);
	}

// RECEIVING THE ECHO STRING FROM THE CLIENT
	while (1) {
		cliAddrLen = sizeof(echoClntAddr);
		if (echoBuffer[0] != '=')
			printf("Handling client...\n");
		recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*) &echoClntAddr, &cliAddrLen);
		if (echoBuffer[0] != '=') {
			char strRecv[ECHOMAX] = "\0";
			float num1, num2, tot;
			extrap(strRecv, echoBuffer, 1);
			num1 = atof(strRecv);
			memset(strRecv, 0, sizeof(strRecv));
			extrap(strRecv, echoBuffer, 2);
			num2 = atof(strRecv);
			memset(strRecv, 0, sizeof(strRecv));

			struct hostent *host;
			struct in_addr addr;
			char *ip = inet_ntoa(echoClntAddr.sin_addr);
			addr.s_addr = inet_addr(ip);
			host = gethostbyaddr((char*) &addr, 4, AF_INET);
			char *nome = host->h_name;
			printf("Operation request '%s' from client %s, ip %s\n", echoBuffer,
					nome, ip);

// OPERATION DEVELOPMENT
			char symbol[2] = "\0";
			char num1C[20] = "\0";
			char num2C[20] = "\0";
			char totC[210] = "\0";
			char operation[ECHOMAX] = "\0";
			sprintf(num1C, "%g", num1);
			sprintf(num2C, "%g", num2);
			if (echoBuffer[0] == '+') {
				tot = add(num1, num2);
				strcat(symbol, "+");
			}
			if (echoBuffer[0] == '-') {
				tot = sub(num1, num2);
				strcat(symbol, "-");
			}
			if (echoBuffer[0] == '*') {
				tot = mult(num1, num2);
				strcat(symbol, "*");
			}
			if (echoBuffer[0] == '/') {
				tot = division(num1, num2);
				strcat(symbol, "/");
			}

			int intpart = (int) tot;
			float decpart = tot - intpart;
			if (decpart != 0.000)
				sprintf(totC, "%.2f", tot);
			else
				sprintf(totC, "%g", tot);
			strcat(operation, num1C);
			strcat(operation, " ");
			strcat(operation, symbol);
			strcat(operation, " ");
			strcat(operation, num2C);
			strcat(operation, " ");
			strcat(operation, "=");
			strcat(operation, " ");
			strcat(operation, totC);
			strcpy(echoBuffer, operation);

// SEND THE ECHO STRING BACK TO THE CLIENT
			if (sendto(sock, echoBuffer, strlen(echoBuffer), 0,
					(struct sockaddr*) &echoClntAddr, sizeof(echoClntAddr))
					!= strlen(echoBuffer)) {
				ErrorHandler(
						"sendto() sent different number of bytes than expected");
				system("pause");
				exit(EXIT_FAILURE);
			}
			for (int i = 0; i < ECHOMAX; i++) {
				echoBuffer[i] = '\0';
			}
		}
	}
}
