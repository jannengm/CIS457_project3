#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"

int main( int argc, char * argv[] ) {
    char buffer[BUFF_SIZE], input[LINE_SIZE], line[LINE_SIZE];
    int err, sockfd, flag, buff_len, file_size, i;
    struct sockaddr_in serveraddr;
    FILE *fp;
    pid_t pid;

    /*Create socket*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("There was an error creating the socket\n");
        exit(SOCKET_CREATE_ERROR);
    }

    /*Check for command line arguments*/
    if (argc < CLIENT_ARGS) {
        printf("Invalid arguments. ");
        printf("Args should be of form: ");
        printf("[port] [IP address]\n");
        exit(INVALID_ARG_ERROR);
    }

    /*Get server port number*/
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( atoi(argv[1] ) );
    serveraddr.sin_addr.s_addr = inet_addr( argv[2] );

    /*Connect to server*/
    err = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (err < 0) {
        printf("There was an error with connecting\n");
        exit(CONNECT_ERROR);
    }

    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork error\n");
        exit(1);
    }
    else if(pid == 0){
        /*Clear input buffer*/
        memset(line, 0, LINE_SIZE);

        recv(sockfd, line, LINE_SIZE, 0);

        printf("%s", line);

        exit(0);
    }
    else{
        /*Clear input buffer*/
        memset(input, 0, LINE_SIZE);

        printf("Enter input: ");

        /*Read from stdin, check errors*/
        if( fgets(input, LINE_SIZE, stdin) == NULL ){
            printf("Input error\n");
            return FILE_INPUT_ERROR;
        }

        /*If no error, send string to server*/
        send(sockfd, input, strlen( input ), 0);

        exit(0);
    }

//    /*Clear input buffer*/
//    memset(input, 0, LINE_SIZE);
//
//    printf("Enter input: ");
//
//    /*Read from stdin, check errors*/
//    if( fgets(input, LINE_SIZE, stdin) == NULL ){
//        printf("Input error\n");
//        return FILE_INPUT_ERROR;
//    }
//
//    /*If no error, send string to server*/
//    send(sockfd, input, strlen( input ), 0);
//
//    return 0;
}