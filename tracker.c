#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>

// Defenitions
#define HEARTBEATMESSAGE "3993"

// Global Variables
struct addrinfo *p;
int sockfd;                     // socket file descriptor for server's heartbeat


// Users Linked List
struct user
{
   char             username[50];
   char             ip[INET6_ADDRSTRLEN];
   int              port;
   struct user      *next;
};


// Functions
void heartbeat()
{
    int numbytes;
    if ((numbytes = sendto(sockfd, HEARTBEATMESSAGE, strlen(HEARTBEATMESSAGE), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("tracker: sendto");
        exit(1);
    }
    alarm(1);
}

bool checkInput(int argc, char *argv[])
{
    if (argc != 5) {
        return false;
    }
    if (strcmp(argv[1], "--server-broadcasts-to") != 0) {
        return false;
    }
    if (strcmp(argv[3], "--clients-broadcasts-to") != 0) {
        return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
    char* server_broadcast_port, clients_broadcast_port;
    // int sockfd;
    struct addrinfo hints, *servinfo;
    int rv;
    int numbytes;

    if (!checkInput(argc, argv)) {
        fprintf(stderr,"usage: server --server-broadcasts-to \"PORT M\" --clients-broadcasts-to \"PORT N\"\n");
        exit(1);       
    }

    server_broadcast_port = argv[2];
    // clients_broadcast_port = argv[4];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo("localhost", server_broadcast_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }


    // Sending heartbeats every 1 seconds
    signal(SIGALRM, heartbeat);
    printf("Server started broadcasting\n");
    alarm(1);
    while (1) {
    }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfd);

    return 0;
}