/*
 * The basic udp echo server program
 */

#include<stdio.h>
#include<string.h>
// #include<conic.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

#define BUFLEN 512  /* maximum buffer size */

long add(long data1, long data2){
	return (data1 + data2);
}
long sub(long data1, long data2){
	return (data1 - data2);
}

long mult(long data1, long data2){
	return (data1 * data2);
}
double divition(long data1, long data2){
	return (data1 / data2);
}

int main(int argc, char **argv)
{
	struct sockaddr_in saddr;		/* server address */
	struct sockaddr_in caddr;		/* client address */
	uint16_t port;					/* port is a 16 bit unsigned integer */
	int sockfd;						/* the socket file descriptor */
	unsigned int addrlen;			/* address length */
	unsigned int recv_len;			/* received data length */
	unsigned int send_len;			/* sending data length */
	char rbuf[BUFLEN];				/* buffer for receiving data of (maximum) size BUFLEN */
	char sbuf[BUFLEN];				/* buffer for sending data of (maximum) size BUFLEN */
	int ret_bind;					/* return value for bind */
	int ret_send;					/* return value for bind */
	char *ptr;

	if (argc != 2)
	{
		fprintf(stderr, "[Usage: %s <server_port>], line %d, file %s.\n", argv[0], __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}

	/* string to integer conversion */

	port = atoi(argv[1]);

	if (port <= 0 || port >65536)
	{
		fprintf(stderr, "invalid port %s, line %d, file %s.\n", argv[1], __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}

	addrlen = sizeof(caddr);

	/* create a udp socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sockfd == -1)
	{
		fprintf(stderr, "socket creation failed, line %d, file %s.\n", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}

	/* zero out the structure to avoid garbages */

	memset((char *) &saddr, 0, sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* bind the socket to a port */

	ret_bind = bind(sockfd , (struct sockaddr*)&saddr, sizeof(saddr));

	if( ret_bind == -1)
	{
		fprintf(stderr, "socket bind failed on port %d, line %d, file %s.\n", port, __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}

	/* The ultimate never ending loop */


	while(1)
	{
		printf("Waiting for data...");

		/* the blocking recvfrom call */
		recv_len = recvfrom(sockfd, rbuf, BUFLEN, 0, (struct sockaddr *) &caddr, &addrlen);

		if ((recv_len == -1))
		{
			fprintf(stderr, "recvfrom() failed, line %d, file %s.\n", __LINE__, __FILE__);
			exit(EXIT_FAILURE);
		}

		/************************ Will vary from problem to problem ********************************/
																									
		/* temporary memory for storing data from buffer for immediate processing.				
		 * although expensive in performance, this is intended here for convenience.*/				
																									
		ptr = malloc(recv_len +1);	

							
												
		strncpy(ptr, rbuf, recv_len);			
													
		ptr[recv_len] = '\0';				
														
																									
		/* printing the received data */						
									
		printf("Received packet from %s:%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));	
		printf("Data: %s, size %d\n" , ptr, recv_len);								
																									
																									
		/* redundant code, but intended for separating sbuf, ptr and rbuf all. */		
																				
		send_len = recv_len;			/* need to adjust this according of problems */				
		strncpy(sbuf, ptr, send_len);	/* populating sbuf */										
			
		// Method Function

		char *token = strtok(sbuf,"/");
		char *value[2];
		int i = 0;
		while(token != NULL)
		{
			value[i++] = token;
			token = strtok(NULL,"/");
		}
		long value1 = atoi(value[0]);
		long value2 = atoi(value[1]);

		long addValue = add(value1, value2);
		long subValue = sub(value1, value2); 
		long multValue = mult(value1, value2);
		double divValue = divition(value1, value2);

		char temp[100];

		sprintf(temp, "Additional Value = %ld -Substraction Value = %ld -Multiplication Value = %ld -Divitional Value = %0.2lf", 
		addValue, subValue, multValue, divValue);

		int templen = strlen(temp);
		//printf("Additional Data : %s \n", temp);
		//printf("Additional Data Length: %d \n", templen);
			
																									
		/************************ Will vary from problem to problem *********************************/

		/* reply back to the client with data */

		ret_send = sendto(sockfd, temp, templen, 0, (struct sockaddr*) &caddr, addrlen);
		if (ret_send == -1)
		{
			fprintf(stderr, "sendto() failed, line %d, file %s.\n", __LINE__, __FILE__);
			exit(EXIT_FAILURE);
		}

		free(ptr);
	}

	close(sockfd);

	exit(EXIT_SUCCESS);
}
