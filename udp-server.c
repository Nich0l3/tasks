#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/types.h> // typedef of - in_addr_t, ip_addr_t : internet address
#include <arpa/inet.h> 
#include <unistd.h>

#define SA struct sockaddr

#define PORT 8080
#define MAX 1024

//Driver Code
int main(){
	
	int sockfd;
	char buffer[MAX];
	char *hello = "Hello from server";
	struct sockaddr_in servaddr, cliaddr;

	// Socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Socket Creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr)); // equivalent of bzero()
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	// binding
	if ( bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0){
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;	
	
	len = sizeof(cliaddr); // len is value/result
	
	n = recvfrom( sockfd, (char *) buffer, MAX, MSG_WAITALL, (SA*) &cliaddr, &len); // cliaddr ?
// MSG_WAITALL flag requests that the operation block wait until the full request is satisfied
// return length of the message.

	buffer[n] = '\0'; // setting nth array-bit to null to complete a string.

	printf("Client : %s\n", buffer);
	sendto( sockfd, (char *)hello, strlen(hello), MSG_CONFIRM, (SA *) &cliaddr, len);
	// MSG_CONFIRM tell the link layer that forward progress happened: you got a successful reply from the other side. If the link layer doesn't get this it will regularly reprobe the neighbor.
	// return number of char sent
	
	printf("Hello message sent.\n");

	return 0;
}
