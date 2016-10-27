#include "client.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
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

int udp_listener(char* server_broadcast_port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    char s[INET6_ADDRSTRLEN];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // my IP

    if ((rv = getaddrinfo(NULL, server_broadcast_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }


        int yes=1;

        // lose the pesky "Address already in use" error message
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
            perror("setsockopt");
            exit(1);
        }


        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        } 

        break;
    }


    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);
    
    printf("listener: waiting to recvfrom...\n");

    return sockfd;

}


int main(int argc, char *argv[])
{
    // Ports
    char* server_broadcast_port;
    char* clients_broadcast_port;

    fd_set master;    // master file descriptor list //---------------------------
    fd_set read_fds;  // temp file descriptor list for select()--//--------------------

    // int nbytes, fdmax,i;
    // int sockfd, numbytes,ss;  
    // char buf[MAXDATASIZE];
    // struct addrinfo hints, *servinfo, *p;
    // int rv;
    // char s[INET6_ADDRSTRLEN];

    // FD_ZERO(&master);    // clear the master and temp sets
    // FD_ZERO(&read_fds); 



    if (!check_input(argc, argv)) {
        fprintf(stderr,"usage: client --server-broadcasts-to \"PORT M\" --clients-broadcasts-to \"PORT N\"\n");
        exit(1);       
    }
    server_broadcast_port = argv[2];
    clients_broadcast_port = argv[4];


    char buf[MAXBUFLEN];
    char s[INET6_ADDRSTRLEN];

    int numbytes;

    struct sockaddr_storage their_addr;
    // int hb_sockfd;
    bool server_available = false;
    bool hb_alive = false;    

    int sockfd;
    sockfd = udp_listener(server_broadcast_port);
    // printf("kir\n");
    // printf("%d\n", sockfd);
    socklen_t addr_len;


    
    addr_len = sizeof their_addr;
    while(1) {
        // printf("%d\n", numbytes);
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
            (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }
        
        // printf("%d\n", numbytes);
        printf("listener: got packet from %s\n",
            inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof s));
        printf("listener: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        printf("listener: packet contains \"%s\"\n", buf);
    }

    close(sockfd);

    return 0;
}