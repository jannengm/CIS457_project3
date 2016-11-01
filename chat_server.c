#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LINE_SIZE 4096
#define BUFF_SIZE 256
#define MAX_CLIENTS 128
#define MAX_QUEUE 10
#define TRUE 1

struct client_t{
    int client_sock;
    int id;
};

typedef struct client_t client_t;

int handle_client( int fd, client_t * clients, int num_clients );

int main( int argc, char * argv[] ){
    int sockfd, i, len, new_sock, num_clients = 0;
    fd_set active_set, read_set;
    struct sockaddr_in serveraddr, clientaddr;
    client_t clients[MAX_CLIENTS];

    /*Create TCP Socket*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Socket create error");
        exit(1);
    }

    /*Check command line args*/
    if(argc < 1){
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    /*Set up server address and port*/
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[1]));
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    /*Bind socket to the address of the server*/
    bind(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));

    /*Listen on socket for up to 10 connections in queue*/
    listen(sockfd, MAX_QUEUE);

    /*Initialize active socket set*/
    FD_ZERO (&active_set);
    FD_SET (sockfd, &active_set);

    /*Loop continously until killed with ctrl+C */
    while(TRUE) {
        read_set = active_set;
        if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
            printf("Select failure\n");
            exit(1);
        }

        /*Cycle through all sockets in read_fd_set*/
        for(i = 0; i < FD_SETSIZE; i++){

            /*Check if socket has pending input*/
            if( FD_ISSET(i, &read_set) ) {
                /*If connection request on master socket (sockfd), create new
                 *socket and add to the active_fd_set*/
                if (i == sockfd) {
                    len = sizeof(clientaddr);
                    new_sock = accept(sockfd, (struct sockaddr *) &clientaddr, &len);
                    if (new_sock < 0) {
                        fprintf(stderr, "Error connecting to client\n");
                        exit(1);
                    }
                    clients[num_clients].client_sock = new_sock;
                    clients[num_clients].id = num_clients;
                    num_clients++;
                    printf("Successfully connected to client\n");
                    FD_SET (new_sock, &active_set);
                }

                    /*If client is connected but not on master socket,
                     *it is ready for use*/
                else {
                    handle_client(i, clients, num_clients);
                    printf("Closing client socket\n");
                    close(i);
                    FD_CLR(i, &active_set);
                }
            }
        }
    }

    return 0;
}

int handle_client( int fd, client_t * clients, int num_clients ){
    char buffer[BUFF_SIZE], line[LINE_SIZE];
    int buff_len, file_size;
    FILE *fp;

    /*Clear the input buffer*/
    memset(line, 0, LINE_SIZE);

    /*Get input from client*/
    recv(fd, line, LINE_SIZE, 0);

//    printf("Got from client:\n%s\n", line);

    /*Administrative commands*/
    if(line[0] == '\\'){
        return 0;   //command exit code?
    }

    /*Select client destination*/

    /*Send message*/

    /*Broadcast to all clients*/
    int i;
    for(i = 0; i < MAX_CLIENTS && i < num_clients; ++i) {
        if(i != clients[i].id) {
            send(clients[i].client_sock, line, strlen(line), 0);
        }
    }
    return 0;
}