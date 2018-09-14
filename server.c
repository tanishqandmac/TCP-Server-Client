#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8000

int main(){
	int sockfd, ret ,newSocket;
	struct sockaddr_in serverAddr;

	struct sockaddr_in newAddr;

	socklen_t addr_size;
 	int i;
	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	
	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}
	char buffer1[1024];
	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, "exit()") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Recieved from Client %d : %s",ntohs(newAddr.sin_port), buffer);			
					sprintf(buffer1, "Recieved from Client %d: %s",(newAddr.sin_port),buffer  );
					send(newSocket, buffer1, strlen(buffer1), 0);
					bzero(buffer, sizeof(buffer));
					bzero(buffer1, sizeof(buffer1));
					printf("\n");
				}
			}
		}

	}

	close(newSocket);
	return 0;
}