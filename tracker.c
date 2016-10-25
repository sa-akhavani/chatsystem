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

#define HEARTBEATMESSAGE "i am alive"

struct sockaddr_in their_addr; // connector's address information
int sockfd;

void heartbeat()
{
    int numbytes;
    if ((numbytes=sendto(sockfd, HEARTBEATMESSAGE, strlen(HEARTBEATMESSAGE), 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        perror("sendto");
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
    int server_broadcast_port, clients_broadcast_port;
    struct hostent *he;
    int numbytes;
    int broadcast = 1;
    //char broadcast = '1'; // if that doesn't work, try this

    if (!checkInput(argc, argv)) {
        fprintf(stderr,"usage: server --server-broadcasts-to \"PORT M\" --clients-broadcasts-to \"PORT N\"\n");
        exit(1);       
    }

    server_broadcast_port = atoi(argv[2]);
    clients_broadcast_port = atoi(argv[4]);

    if ((he=gethostbyname("localhost")) == NULL) {  // get the host info
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // this call is what allows broadcast packets to be sent:
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }

    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(server_broadcast_port); // short, network byte order
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);


    printf("started sending heartbeats\n");
    
    // Sending heartbeats every 1 seconds
    signal(SIGALRM, heartbeat);
    alarm(1);
    while(1) {
    }



    printf("sent %d bytes to %s\n", numbytes,
        inet_ntoa(their_addr.sin_addr));

    close(sockfd);

    return 0;
}