#include "tracker.h"

// Functions
void heartbeat()
{
    int numbytes;
    if ((numbytes = sendto(heartbeatfd, HEARTBEATMESSAGE, strlen(HEARTBEATMESSAGE), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("tracker: sendto");
        exit(1);
    }
    alarm(1);
}

bool check_input(int argc, char *argv[])
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


void logout ()
{

}

void login ()
{
    // current->next = malloc(sizeof(struct user));
}


int main(int argc, char *argv[])
{
    char* server_broadcast_port;
    char* clients_broadcast_port;
    // int heartbeatfd;
    struct addrinfo hints, *servinfo;
    int rv;
    int numbytes;

    if (!check_input(argc, argv)) {
        fprintf(stderr,"usage: server --server-broadcasts-to \"PORT M\" --clients-broadcasts-to \"PORT N\"\n");
        exit(1);       
    }

    server_broadcast_port = argv[2];
    clients_broadcast_port = argv[4];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo("localhost", server_broadcast_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((heartbeatfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("tracker: socket");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "tracker: failed to create socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    // Sending heartbeats every 1 seconds
    signal(SIGALRM, heartbeat);
    printf("Server started broadcasting\n");
    alarm(1);
    while (1) {
    }



    printf("tracker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(heartbeatfd);

    return 0;
}