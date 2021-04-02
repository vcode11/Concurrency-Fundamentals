#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
void error(const char *msg){
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])
{
	if(argc < 2){
		error("Port not provided");
	}
	int sockfd, newsockfd, portno, n;
	char buffer[255];

	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Failed to create a socket.\n");
	}
	bzero((char *) &server_addr, sizeof(server_addr));
	portno = atoi(argv[1]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		error("Binding Failed");
	}
	listen(sockfd, 5);
	client_len = sizeof(client_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &client_addr, &client_len);
	if(newsockfd < 0){
		error("Could not accept\n");
	}
	while(1){
		bzero(buffer, 255);
		n = read(newsockfd, buffer, 255);
		if(n < 0) error("Read failed\n");
		printf("Client: %s\n", buffer);
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(newsockfd, buffer, strlen(buffer));
		if(n < 0) error("Read failed\n");
		if(strncmp("bye", buffer, 3) == 0){
			break;
		}
	}
	close(newsockfd);
	close(sockfd);
}
