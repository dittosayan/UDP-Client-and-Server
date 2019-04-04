// udp client driver program 
// ./clientPacketGen <ServerIP> <ServerPort> <P> <TTL> <NumPackets>
#include <fcntl.h>
#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>

#define IPSIZE 12
#define TS_SIZE 6

int port,payload,ttl,packets;

void fillPacket(char* message,int seq)
{
	struct timeval tv;
	int i;
	unsigned long int tu,tu2;

	gettimeofday(&tv,NULL);
	tu=tv.tv_usec;

	char ustring[TS_SIZE];

	sprintf(ustring,"%lx",tu);

	//printf("Timestamp : %lx\n",tu);
	//printf("USTRING : %s\n\n",ustring);

	//Seq. No.
	message[0]='x';
	message[1]=seq;

	//Timestamp
	message[2]=ustring[1];
	message[3]=ustring[2];
	message[4]=ustring[3];
	message[5]=ustring[4];

	//TTL
	message[6]=ttl;

	//payload
	for(i=0;i<payload;i++)
		message[7+i]='x';

	message[7+i]='\0';
}

long int calculate_rtt(char* message)
{
	struct timeval tv;
	char a[5],b[5],c[5];
	unsigned long int tsa,tsb;
	unsigned long int tu;

	long int result;

    char *ptr;
    //long ret;

   	//ret = strtoul(str, &ptr, 10);
	//sprintf(ts,"%s",tu);

	memcpy (a, message+2, sizeof(char)*4);
	a[4]='\0';
	// printf("Timestamp obt: %s\n\n",a);
	

	gettimeofday(&tv,NULL);
	tu=tv.tv_usec;

	sprintf(b,"%lx",tu);

	memcpy(c, b+1, sizeof(char)*4);
	c[4]='\0';

	// printf("Timestamp now: %s\n\n",c);

	//tsa=atoi(a);
	tsa=strtoul(a, &ptr, 16);
	// printf("tsa : %lx\n\n",tsa);
	tsb=strtoul(c, &ptr, 16);
	// printf("tsb : %lx\n\n",tsb);

	// printf("RTT : %lx\n\n",tsb-tsa);
	result=tsb-tsa;

	return(result);
}

int main(int argc, char const *argv[]) 
{  
	FILE *fp,*fp2;

	fp=fopen(argv[6],"a");
	// fp2=fopen(argv[7],"w+");

	int sockfd,fd; 
	struct sockaddr_in servaddr;

	int  n, i, j;
	long int rtt,avg=0;
	
	char ip[IPSIZE];
	strcpy(ip,argv[1]);

	port=atoi(argv[2]);

	payload=atoi(argv[3]);

	ttl=atoi(argv[4]);

	packets=atoi(argv[5]);

	
	char fn[10];
	
	// char sys_call[20]="chmod 777 ";
	
	// strcpy(fn,argv[6]);
	
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_addr.s_addr = inet_addr(ip); 
	servaddr.sin_port = htons(port); 
	servaddr.sin_family = AF_INET; 
	
	// create datagram socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	// connect to server 
	// if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	// { 
	// 	printf("\n Error : Connect Failed \n"); 
	// 	exit(0); 
	// } 

	// fd=open(argv[6],O_RDWR | O_CREAT);
	
	// // for(i=0;i<strlen(fn);i++)
	// // 	sys_call[10+i]=fn[i];

	// // sys_call[i]='\0';

	// // printf("sys_call : %s\n",sys_call);
	// system("chmod 777 dt.txt");
	
	// printf("%d\n",fd);
	// write(fd, "hello geeks\n", strlen("hello geeks\n"));
	
	// close(fd);

	char buffer[payload+7]; 
	char message[payload+7];

	// for(int k=1;k<=2;k++)
	//  {
	// 	//printf("------------K = %d\n",k);
	// 	payload=100*k;
	// 	printf("Payload : %d\n",payload);

		

		for(j=0;j<packets;j++)
		{
			memset(message,0,sizeof(message));
			memset(buffer,0,sizeof(buffer));

			printf("Sequence no. : %d\n\n",j+1);
			fillPacket(&message,j+1);
			
			while(1)
		    { 
		    	//printf("TTL sent to server : %d\n\n",message[6]);

		    	sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)(&servaddr), sizeof(servaddr)); 
				
				// waiting for response 
				n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)(&servaddr), sizeof(servaddr)); 
				//puts(buffer);
				
				buffer[n] = '\0'; 
		        
		        //printf("Server : %s\n", buffer);
		        //printf("Bytes received : %d\n",n);
		        for(i=0;i<n;i++)
		            message[i]=buffer[i];

		        message[6]=(int)message[6]-1;
		        if((int)(message[6])==0)
		        	break;
			}

			rtt = calculate_rtt(message);

			// fprintf(fp,"%d %ld\n",payload, rtt);
			// printf("RTT of seq. no. %d : %ld useconds\n\n",j+1,rtt);
			// printf("%ld,",rtt);
			avg+=rtt;

			if(((j+1)%10)==0)
				printf("%d %ld\n",j+1,rtt);
		} 

	// printf("\n");
	

	// for(int k=1;k<10;k++)
	// {
	// 	for(int i=1;i<=50;i++)
	 		printf("%d,",i);
	// }

	avg/=packets;
	fprintf(fp,"%d %ld\n",payload, avg);
	printf("Avg. RTT for all the packets is : %ld useconds\n\n",avg);
	
	// close the descriptor 
	message[0]='c';
	message[1]='\0';
	
	sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	close(sockfd);
}
