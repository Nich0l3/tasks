// adding the necessary libraries.
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr


void func(int sockfd){
	
	char buffer[MAX];
	int n;

	for (;;){
		
		bzero(buffer, sizeof(buffer));

		printf("Enter the string: ");
		n = 0;

		while((buffer[n++] = getchar()) != '\n');

		// sending message
		write (sockfd, buffer, sizeof(buffer));
		bzero(buffer, sizeof(buffer));

		//readin the message
		read(sockfd, buffer, sizeof(buffer));
		printf("From the server : %s\n", buffer);
		
		if(strncmp("exit",buffer,4) == 0){
			printf("Closing client\n");
			break;
		}
	}
}

int main(){
	
	int sockfd, connfd;
	struct sockaddr_in socketAddress, cli;

	// socket creation and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1){
		printf("Socket not created!\n");
		exit(0);
	}
	else
		printf("Socket successfully created !\n");

	bzero(&socketAddress, sizeof(socketAddress));
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	socketAddress.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&socketAddress, sizeof(socketAddress)) != 0){
		printf("Connection with the server failed!\n");
		exit(0);
	}
	else
		printf("Connect to the server!\n");

	func(sockfd);

	close(sockfd);
}	
