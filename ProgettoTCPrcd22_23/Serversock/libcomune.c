#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>

void errorhandler(char *errorMessage){
	printf("%s", errorMessage);
}

void clearwinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

