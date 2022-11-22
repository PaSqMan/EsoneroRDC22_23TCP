
#include "libcomune.h"
#include <ctype.h>
#include <time.h>
#define QLEN 5

void dividetimpera(char operator[2],int*first,int*second,char*stringfclient);
int sendtoclient(int my_socketftf,float*resultftf);
void add(int*first,int*second,float*result);
void sub(int*first,int*second,float*result);
void mult(int*first,int*second,float*result);
void division(int*first,int*second,float*result);
void Countdown(int j);

int main(int argc, char *argv[]) {
	int port;
	if (argc > 1) {
		port = atoi(argv[1]);

	} else
		port = PROTOPORT;
	if (port < 0) {
		printf("bad port number %s \n", argv[1]);
		return 0;
	}
	#if defined WIN32
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		errorhandler("Error at WSAStartup()\n");
		return 0;
	}
	#endif
	//SOCKET CREATION
	int my_socket;
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_socket < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return -1;
	}
    //BINDING SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port);
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("bind() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return -1;
	}
    //SOCKET SETTING AT LISTENING
	if (listen(my_socket, QLEN) < 0) {
		errorhandler("listen() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return -1;
	}
    //ACCEPTING NEW CONNECTION
	struct sockaddr_in cad;
	int client_socket;
	int client_len;
	while (1) {
		printf("Waiting for a client to connect. . .");
		client_len = sizeof(cad);
		if ((client_socket = accept(my_socket, (struct sockaddr*) &cad,
				&client_len)) < 0) {
			errorhandler("accept() failed.\n");
			//CLOSING CONNECTION
			closesocket(client_socket);
			clearwinsock();
			return 0;
		}
		char*ip=inet_ntoa(cad.sin_addr);
		unsigned int port=htons(cad.sin_port);
		printf("Connection established with %s:%d\n",ip,port);
		char stringserver[20]={'\0'};
		char op[2]={"\0"};
		while(op[0]!='='){
			if(recv(client_socket,stringserver,sizeof(stringserver),0)<=0){
				errorhandler("\nConnection closed prematurely by client or recv() failed.\n\n");
				Countdown(5);
				break;
			}
			int fir=0;
		    int sec=0;
			float result=0;
			char msgexit[20]="Goodbye ;)";
			char errormsg[20]="Invalid operator :(";
			char erroropmsg[20]="Invalid 2^number :(";
			dividetimpera(op,&fir,&sec,stringserver);
			switch(op[0]){
				case'+':
					add(&fir,&sec,&result);
					if(sendtoclient(client_socket,&result)==-1){
						errorhandler("send() failed");
					}
					memset(op,'\0',2);
				break;
				case'-':
					sub(&fir,&sec,&result);
					if(sendtoclient(client_socket,&result)==-1){
						errorhandler("send() failed");
					}
					memset(op,'\0',2);
				break;
				case'x':
					mult(&fir,&sec,&result);
					if(sendtoclient(client_socket,&result)==-1){
						errorhandler("send() failed");
					}
					memset(op,'\0',2);
				break;
				case'/':
					if(sec!=0){
						division(&fir,&sec,&result);
						if(sendtoclient(client_socket,&result)==-1){
							errorhandler("send() failed");
						}
					}
				    else{
				    	if(send(client_socket,erroropmsg,sizeof(erroropmsg),0)<=0){
				    		errorhandler("send() failed");
				        }
				    }
				    memset(op,'\0',2);
				break;
				case'=':
					if(send(client_socket,msgexit,sizeof(msgexit),0)<=0){
						errorhandler("send() failed");
					}
				break;
				default:
					if(send(client_socket,errormsg,sizeof(errormsg),0)<=0){
						errorhandler("send() failed");
					}
					memset(op,'\0',2);
				break;
			}
		}
		system("cls");
	}
}

void add(int*first,int*second,float*result){
	*result=(*first)+(*second);
}
void sub(int*first,int*second,float*result){
	*result=(*first)-(*second);
}
void mult(int*first,int*second,float*result){
	*result=(*first)*(*second);
}
void division(int*first,int*second,float*result){
	*result=(float)(*first)/(*second);
}

int sendtoclient(int my_socketftf,float*resultftf){
	char results[10]={'\0'};
	sprintf(results, "%f", *resultftf);
	if(send(my_socketftf,results,sizeof(results),0)<=0){
		return -1;
	}
	return 0;
}

void dividetimpera(char operator[2],int*first,int*second,char*stringfclient){
	unsigned int y=0;
	char delimiter[2]=" ";
	char *token=strtok(stringfclient,delimiter);
	while(token!=NULL){
		y++;
	    switch(y){
	    	case 1:
	        strcpy(operator,token);
	        break;
	        case 2:
	         *first=atoi(token);
	        break;
	        case 3:
	         *second=atoi(token);
	        break;

	    }
	    token=strtok(NULL,delimiter);
    }
}

void Countdown(int j){
	printf("Rebooting in:\n\n");
	for(;j>=0;j--){
		Sleep(1000);
		printf("%u\n\n",j);
	}
	Sleep(1000);
}

