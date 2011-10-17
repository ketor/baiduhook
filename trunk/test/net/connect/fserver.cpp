#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define SERVPORT        9999
#define MAXDATASIZE     256
#define	BACKLOG		10
#define STDIN           0

int main(void)
{
	int sockfd, client_fd;
	socklen_t sin_size;
	struct sockaddr_in my_addr, client_addr;
	char buf[256];
	//char send_str[256];
	int recvbytes;
	pid_t pid;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket");
		exit(1);
	}
	
	bzero(&my_addr, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}

	printf("start.....\n");
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) 
		{
			perror("accept");
			continue;
		}
		pid = fork();
		if(pid == 0)
		{
			close(sockfd);
			recvbytes = recv(client_fd, buf, MAXDATASIZE, 0);
			if(recvbytes <= 0) {
				printf("\e[31;1m[=FAIL=] recv fail: [%d]%s\e[m\n", errno, strerror(errno));
			} else {
				buf[recvbytes] = '\0';
				printf("\e[32;1m[==OK==] recv buf: %s, addr:[%s:%d]\n\e[m", 
					buf, inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
			}

			close(client_fd);
			exit(1);
		} 
		wait(NULL);
	}	
	
	return 1;
}
