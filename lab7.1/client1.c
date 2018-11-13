/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct{
	char operation;
	int values[10];
	int size;
} package;

int main(int argc, char *argv[])
{
    int sockfd;
    int len,i,size;
    struct sockaddr_un address;
    long result;
    package pack;

    pack.operation = *argv[1];
    pack.size = argc - 2;
    //printf("Operation: %c\n",pack.operation);
    for(i=2;i<argc;i++){
	pack.values[i-2] = atoi(argv[i]);
	//printf("Pack[%d] = %d\n",i-2,pack.values[i-2]);
    }

/*  Create a socket for the client.  */

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }

/*  We can now read/write via sockfd.  */
    write(sockfd, &pack, sizeof(pack));
    read(sockfd, &result, sizeof(long));
    if(result == -99999) printf("Client: unsupported operation\n");
    else printf("Client: result from server = %ld\n", result);
    close(sockfd);
    exit(0);
}
