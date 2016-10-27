#ifndef TRACKER_H
#define TRACKER_H

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
#define HEARTBEATMESSAGE "6063" // tells to the clients which port it is listening
#define TRACKERLISTENINGPORT "6063" // which port it is listening

struct addrinfo *udp_servinfo, *udp_p;

// Users Linked List
struct user
{
   char*             username;
   char             ip[INET6_ADDRSTRLEN];
   char*              port;
   struct user      *next;
};



// Functions
// int heartbeat(struct addrinfo *hints, char* server_broadcast_port)
bool check_input(int argc, char *argv[]);
// void print_all(struct user * first);

#endif