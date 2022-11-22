#include "libcomune.h"

int main(void) {

	#if defined WIN32
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		printf("error at WSAStartup\n.");
		return -1;
	}
	#endif
	//SOCKET CREATION
	int c_socket;
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_socket < 0) {
		errorhandler("socket creation failed.\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(PROTOPORT);

	//CONNECTING TO THE SERVER
	if (connect(c_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("Failed to connect.\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	char stringclient[20]={'\0'};
	char stringrecieved[20]={'\0'};
	char mexendsending[20]="Goodbye ;)";
	char mexerror[20]="Invalid operator :(";
	char erroropmsg[20]="Invalid 2^number :(";
	printf("Enter the operation you want to process: (ex.'+ 25 26[Enter]');\nOperator '=[Enter]' TO EXIT;");
	while(strcmp(stringrecieved,mexendsending)!=0){
		printf("\n?");
		gets(stringclient);
		fflush(stdin);
		if(send(c_socket,stringclient,sizeof(stringclient),0)<0){
			errorhandler("send() failed");
			break;
		}
		if(recv(c_socket,stringrecieved,sizeof(stringrecieved),0)<0){
			errorhandler("recv() failed");
			break;
		}
		if((strcmp(stringrecieved,mexendsending)!=0)&&(strcmp(stringrecieved,mexerror)!=0)&&(strcmp(stringrecieved,erroropmsg)!=0)){
			printf("Result : %3.3f",atof(stringrecieved));
		}
		else{
			printf(stringrecieved);
		}
		if(strcmp(stringrecieved,mexendsending)!=0){
			memset(stringrecieved,'\0',20);
		}
	}
	// CLOSING CONNECTION
	closesocket(c_socket);
	clearwinsock();
	printf("\n");
	system("pause");
	return (0);
}

