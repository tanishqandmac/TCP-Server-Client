#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 4444
volatile sig_atomic_t flag = 0;
void my_function(int sig){ // can be called asynchronously
  flag = 1; // set flag
}
int main(){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serverAddr, '\0', sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	while(1){
		signal(SIGINT, my_function); 
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		if((strcmp(buffer, "exit()") == 0) || flag==1){
			close(clientSocket);
			exit(1);
			flag=0;
		}
/*
		if(recv(clientSocket, buffer, 1024, 0) < 0){
			if(strcmp(buffer, "exit()") == 0){
			close(clientSocket);
			printf("Server has been Shut Down\n");
			exit(1);}
		}
*/
	}

	return 0;
}
