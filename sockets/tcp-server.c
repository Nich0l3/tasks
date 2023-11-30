// libraries
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80		// max buffer size
#define PORT 8080
#define SA struct sockaddr // generic socket address

// function for sending and recieving messages
void func(int connectID){
	
	//buffer
	char buffer[MAX];
	int n;

	// infinite loop for chat
	for (;;){
		
		bzero(buffer, MAX); // erase the given memory address with zero bytes 

		// reading the message
		read(connectID, buffer, sizeof(buffer));

		printf("From the client: %s\t To the client :", buffer);

		bzero(buffer, MAX); // clearing buffer for new message

		n = 0;
		while ((buffer[n++] = getchar()) != '\n') ; // copying server message to buffer

		// writing the message
		write(connectID, buffer, sizeof(buffer)); 

		//exiting
		if (strncmp("exit",buffer, 4) == 0) {
			
			printf("Closing server");
			break;
		}
	}
}

int main(){
	int sockfd, connectID, length;
	struct sockaddr_in serverAddress, client; // sockaddrr_in has actual IPv4 layout 

	// creating a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket	

	// AF_INET -> doamain, selects an address family used to designate the type of addresses that your socket can communicate with (in this case IPv4). These constants are defined in sys/socket.h
	
	// SOCK_STREAM -> type, specify type of socket created. type is analogous to the type of communication requested. These constants are defined in sys/socket.h
	// vairous supported types are - 
	// 	SOCK_DGRAM  - datagrams (UDP)
	// 	SOCK_STREAM - sequenced two-way byte streams (TCP)
	
	// 0 -> protocol, specifies a particular protocol to be used with the socket. If protocol param set to 0, system selects the default protocol number for the domain and socket type requested.
	
	if (sockfd == -1){
		printf("Socket not created!\n");
		exit(0);
	}
	else
		printf("Socket created successfully \n");

	bzero(&serverAddress, sizeof(serverAddress));
	
	// Assigning IP address, and PORT address 
	serverAddress.sin_family = AF_INET;
	
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // htonl - u_long from host to tcp/ip network byte order (which is big-endian ?). Here htonl function in converting IPv4 address (32-bits) in host byte order to the IPv4 address in network byte order.

	// INNADD_ANY - defined at netinet/in.h 
	// 		When a process wants to receive new incoming packets or connections, it should bind a socket to a local interface address using bind().
	// 		In this case, only one IP socket may be bound to any given local (address, port) pair. When INADDR_ANY is specified in the bind call, the socket will be bound to all local interfaces.
	// 		For a server, you typically want to bind to all interfaces - not just "localhost".
	// 		When listen() or connect() is called upon an unbound socket, the socket is automatically bound to a randon free port or to a usable shared port with the local address set to INADDR_ANY.
	

	serverAddress.sin_port = htons(PORT); // htons - u_short from host to TCP/IP network byte order (which is big-endian ?). Here it convert an IP port number (16-bits) in host byte order to the IP port number in network byte order.
	

	// Socket binding AKA making passive sockets
	if ((bind(sockfd, (SA *)&serverAddress, sizeof(serverAddress))) != 0){ // why use generic socket address pointer ?
		printf("Socket binding failed!\n");
		exit(0);
	}
	else
		printf("Socket binding done successfully\n");


	// Listening
	if((listen(sockfd,5)) != 0){
		printf("Listen failed !\n");
		exit(0);
	}
	else
		printf("Server listening!\n");

	// Accept data from client and verify
	length = sizeof(client);
	connectID = accept(sockfd, (SA*)&client, &length); // Generic socket address ?
	// accpet() - accepts first connection on its queue of pending connections for the given socket, here sockfd. The accept() call creates a new socket descriptor with the same properties as sockfd and returns it to the caller.
	
	if (connectID < 0){
		printf("Server accept failed!\n");
		exit(0);
	}
	else
		printf("Server accepth the client!\n");
	func(connectID);

	//close the connection
	close(sockfd);
}
			
