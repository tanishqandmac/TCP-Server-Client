#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int serverSocket, conntection ,newSocket;
	struct sockaddr_in serverAddr;

	struct sockaddr_in newAddr;

	socklen_t addr_size;
 	int i;
	char message[1000];
	pid_t childpid;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8000);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	conntection = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	
	if(listen(serverSocket, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}
	char message1[1000];
	while(1){
		newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(serverSocket);

			while(1){
				recv(newSocket, message, 1024, 0);
				if(strcmp(message, "exit()") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Recieved from Client %d : %s",ntohs(newAddr.sin_port), message);			
					sprintf(message1, "Recieved from Client %d: %s",(newAddr.sin_port),message  );
					send(newSocket, message1, strlen(message1), 0);
					bzero(message, sizeof(message));
					bzero(message1, sizeof(message1));
					printf("\n");
				}
			}
		}

	}

	close(newSocket);
	return 0;
}