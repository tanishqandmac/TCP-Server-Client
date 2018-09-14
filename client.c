#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

int main(){
	struct sockaddr_in serAddress;
	int clientSocket, connection;
	char message[1000];

	memset(&serAddress,'\0',sizeof(serAddress));
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	serAddress.sin_family = AF_INET;
	serAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serAddress.sin_port = htons(8000);
	connection = connect(clientSocket, (struct sockaddr*)&serAddress, sizeof(serAddress));

	for(;;){
		printf("Client: ");
		scanf("%s", &message[0]);
		send(clientSocket, message, strlen(message), 0);

		if(strcmp(message, "exit()") == 0){
			close(clientSocket);
			exit(1);
		}

		if(recv(clientSocket, message, 1024, 0) >= 0){
			printf("%s\n", message);
			
		}else{
			printf("Error in connecting!\n");
			//printf("Recieved from client %d: %s\n", message);
		}
	}
	return 0;
}
