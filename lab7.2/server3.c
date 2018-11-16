/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(){
    int server_sockfd, client_sockfd,client_ids[5];
    int server_len, client_len,client_cnt = 0;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    time_t raw_time;

/*  Remove any old socket and create an unnamed socket for the server.  */

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket.  */

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

/*  Create a connection queue and wait for clients.  */

    listen(server_sockfd, 5);
    signal(SIGCHLD, SIG_IGN);
    while(1) {

        printf("Server: waiting...\n");

/*  Accept a connection.  */

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, 
            (struct sockaddr *)&client_address, &client_len);
	
	if(fork() == 0){
		read(client_sockfd, &client_ids[client_cnt], sizeof(int));
		printf("Server: Client %d connected\n",client_ids[client_cnt++]);
		do{
			sleep(1);
			time(&raw_time);
			write(client_sockfd, &raw_time, sizeof(time_t));
		}while(1);
		close(client_sockfd);
	}else{
		close(client_sockfd);
	}
    }
    exit(0);
}

