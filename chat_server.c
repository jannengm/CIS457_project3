#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main( int argc, char * argv[] ){
    int sockfd;
    fd_set active_set, read_set;
    struct sockaddr_in serveraddr, clientaddr;

    /*Create TCP Socket*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Socket create error");
        exit(1);
    }

    /*Check command line args*/
    if(argc < 1){
        //stuff
    }

    /*Set up server address and port*/
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5678);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    /*Bind socket to the address of the server*/
    bind(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));

    /**/

}