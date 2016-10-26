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
#define HEARTBEATMESSAGE "3993" // tells to the clients which port it is listening

// Global Variables
struct addrinfo *p;
int heartbeatfd;                     // socket file descriptor for server's heartbeat


// Users Linked List
struct user
{
   char             username[50];
   char             ip[INET6_ADDRSTRLEN];
   int              port;
   struct user      *next;
};



// Functions
void heartbeat();
bool check_input(int argc, char *argv[]);
void print_all(struct user * first);

#endif