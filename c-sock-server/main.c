#include <stdio.h>

// winsock2.h is the header file to be included for winsock functions
#include <WinSock2.h>

#include <WS2tcpip.h>

// ws2_32.lib is the library file to be linked with the program to be able to use winsock functions.
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[]) {

	// This structure is used to store Windows Sockets initialization data returned by a call to WSAStartup. 
	// It contains data about the Winsock.dll implementation.
	WSADATA wsa;

	SOCKET s;

	// IPv4 AF_INET socket
	/*
		short          sin_family;
		u_short        sin_port;
		struct in_addr sin_addr;
		char           sin_zero[8];
	*/
	struct sockaddr_in server;
	char *message;

	printf("\nInitialising Winsock...");

	// if we cannot use winsock vers 2.2
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : &d", WSAGetLastError());
		return 1;
	}

	printf("\nInitialised.");

	// socket(address_family, socket_type, protocol)
	// If a value of 0 is specified, the caller does not wish to specify a protocol 
	// and the service provider will choose the protocol to use.
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket: &d", WSAGetLastError());
	}

	printf("\nSocket created.");

	// the server ip we want to connect to 
	// The ip string can be converted to the in_addr structure with the InetPton function
	// since inet_addr is deprecated and potentially leads to thread safety issues
	InetPton(AF_INET, TEXT("127.0.0.1"), &server.sin_addr.s_addr);
	server.sin_family = AF_INET;

	// htons is done to maintain the arrangement of bytes which is sent in the network(Endianness)
	server.sin_port = htons( 80 );

	// connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		puts("\nConnect error");
		return 1;
	}

	puts("Connected to: &d", server.sin_addr.s_addr);

	//Send some data
	message = "GET / HTTP/1.1\r\n\r\n";
	if (send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");


	return 0;
}