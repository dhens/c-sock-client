#include <stdio.h>

// winsock2.h is the header file to be included for winsock functions
#include <WinSock2.h>

// ws2_32.lib is the library file to be linked with the program to be able to use winsock functions.
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[]) {

	// This structure is used to store Windows Sockets initialization data returned by a call to WSAStartup. 
	// It contains data about the Winsock.dll implementation.
	WSADATA wsa;

	SOCKET s;

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

	return 0;
}