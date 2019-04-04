// server program for udp connection 
#include <stdio.h> 
#include <stdlib.h>	
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>
#include <time.h>

#define PORT     8000 
#define MAXLINE 1024
#define PACKETS 1
#define TTL 20
#define PS 100

// Driver code 
int main(int argc, char const *argv[])
{ 
	char buffer[MAXLINE]; 
	char message[MAXLINE]; 
	int listenfd, len, i, j; 

	struct timeval tv;

	struct sockaddr_in servaddr, cliaddr; 
	bzero(&servaddr, sizeof(servaddr)); 

	int port,flag=0;
	port=atoi(argv[1]);

	// Create a UDP Socket 
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);		 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port); 
	servaddr.sin_family = AF_INET; 

	// bind server address to socket descriptor 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	
	//receive the datagram 
	len = sizeof(cliaddr); 
	
	while(1)
	{
		flag=0;
		while(1)
		{
			int n = recvfrom(listenfd, buffer, sizeof(buffer), 
				0, (struct sockaddr*)&cliaddr,&len); //receive message from server 

			if(n==1)
				exit(0);

			if(!flag)
			{
				printf("Sequence no. : %d\n\n",buffer[1]);
				flag=1;
			}
			//printf("Bytes received : %d\n",n);
			buffer[6]=(int)buffer[6]-1;
			// printf("TTL sent to client : %d\n\n",buffer[6]); 

			for(i=0;i<n;i++)
		        message[i]=buffer[i];
			
			// send the response 
			sendto(listenfd, message, n, 0, 
				(struct sockaddr*)&cliaddr, sizeof(cliaddr));

			if((int)(buffer[6])==1)
				break;
			//printf("Bytes sent : %d\n",n); 
		}
	}
} 
