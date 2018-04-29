#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<strings.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 512  /* maximum length of the buffer */

int main(int argc, char **argv)
{
    struct sockaddr_in saddr;
    int sockfd;
    int i;
    int addrlen;
    uint16_t port;
    char buf[BUFLEN];
    char *message;
    int ret_send;
    char *server_ip;
    int recv_len;
    char *ptr;

	if (argc < 4)
	{
		fprintf(stderr, "[Usage: %s <server_ip> <server_port> <message>], line %d, file %s.\n", argv[0], __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}

    sockfd =socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1)
    {
		fprintf(stderr, "socket creation failed, line %d, file %s.\n", __LINE__, __FILE__);
		exit(EXIT_FAILURE);
    }

	port = atoi(argv[2]);
    printf("port=%d\n", port);
	if (port <= 0 || port >65536)
	{
		fprintf(stderr, "invalid port %s, line %d, file %s.\n", argv[2], __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}

    addrlen=sizeof(saddr);
    memset((char *) &saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    server_ip = argv[1];

    if (inet_aton(server_ip , &saddr.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed, or wrong server ip, line %d, file %s.\n", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    for (i=3; i<argc; i++) /* This loop handles multiple data sending */
    {
        message = argv[i];
        /* send the message*/
        ret_send = sendto(sockfd, message, strlen(message) , 0 , (struct sockaddr *) &saddr, addrlen);
        if (ret_send == -1)
        {
		    fprintf(stderr, "sendto() failed, line %d, file %s.\n", __LINE__, __FILE__);
		    exit(EXIT_FAILURE);
        }

        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);

        /* try to receive some data, this is a blocking call
        * if data is received, then print it. */

        recv_len = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *) &saddr, &addrlen);
        if (recv_len == -1)
        {
		    fprintf(stderr, "recvfrom() failed, line %d, file %s.\n", __LINE__, __FILE__);
		    exit(EXIT_FAILURE);
        }

        /* copy received data to a temporary location */
        ptr = malloc(recv_len+1);
        strncpy(ptr, buf, recv_len+1);

        // printf("Received data: %s\n", ptr);
	
	char *token = strtok(buf, "-");
	while(token != NULL)
	{
		printf("%s \n", token);
		token = strtok(NULL, "-");
	}
	
	// printf("Check data: %s\n", buf);

    }

    close(sockfd);
    exit(EXIT_SUCCESS);
}
