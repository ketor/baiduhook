#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define SERVPORT 	9999
#define MAXDATASIZE 	256
#define STDIN 		0

int main(int argc, char **argv)
{
	int sockfd;
	//int recvbytes;
	//char buf[MAXDATASIZE];
	char send_str[MAXDATASIZE];
	struct sockaddr_in serv_addr;

	bzero(&serv_addr, sizeof(struct sockaddr_in));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERVPORT);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);

	int i = 0;
	while(1) {
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		{
			printf("\e[31;1m[%03d][=FAIL=] create socket fail: [%d]%s\e[m\n", i, errno, strerror(errno));
			goto next;
		}

		int ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
		if(ret == -1) {
			printf("\e[31;1m[%03d][=FAIL=] connect fail: [%d]%s\e[m\n", i, errno, strerror(errno));
			close(sockfd);
			goto next;
		} else {
			printf("\e[32;1m[%03d][==OK==] connect sucess!\n\e[m", i);
		}
		snprintf(send_str, MAXDATASIZE, "[%02d] send data", i);
		ret = send(sockfd, send_str, strlen(send_str), 0);
		
		close(sockfd);
next:
		sleep(1);
		i++;
	}
	return 1;
}
