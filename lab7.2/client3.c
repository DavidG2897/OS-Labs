/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sockfd, len, result,id;
    struct sockaddr_in address;
    time_t time;

    id = atoi(argv[1]);

/*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client3");
        exit(1);
    }

/*  We can now read/write via sockfd.  */

    write(sockfd, &id, sizeof(int));
    do{
	read(sockfd, &time, sizeof(time_t));
 	printf("Client %d: time from server = %s seconds\n", id, asctime(localtime(&time)));
	sleep(1);
    }while(1);
    close(sockfd);
    exit(0);
}
